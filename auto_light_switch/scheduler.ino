
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define CONFIG_POLL_INTERVAL      600000 //  10 minutes
#define SERVO_UPDATE_INTERVAL        500
#define SERVO_MAX_ACTIVATION_TIME    800
#define MAX_TIMES                     32

#define CONFIG_URL "https://raw.githubusercontent.com/hoffmannmatheus/ardu-light-switch/main/schedules/daily"

bool didPollFirstSchedule = false;
unsigned long lastConfigPollTime = 0;
unsigned long lastServoUpdateCheckTime = 0;

struct Schedule {
    String times[MAX_TIMES];
    unsigned int size;
} onSchedule, offSchedule;

void configParserLoop() {
  unsigned long now = millis();
  if ((now - lastConfigPollTime > CONFIG_POLL_INTERVAL) || !didPollFirstSchedule) {
    lastConfigPollTime = now;
    getRemoteConfig();
  };
  if (now - lastServoUpdateCheckTime > SERVO_UPDATE_INTERVAL) {
    lastServoUpdateCheckTime = now;
    updateServoIfNeeded();
  }
}

void getRemoteConfig() {
  didPollFirstSchedule = true;
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected, maybe next time!");
    setColor(COLOR_RED);
    return;
  }
  Serial.println("Polling configuration...");

  HTTPClient http;
  String path = CONFIG_URL;
  http.begin(path.c_str());

  int httpResponseCode = http.GET();
  String payload;
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  switch (httpResponseCode) {
  case 200:
    payload = http.getString();
    setupTimeSchedules(payload);
    break;
  default:
    setColor(COLOR_RED);
    Serial.println("Error...");
    break;
  }

  http.end();
}

void setupTimeSchedules(String payload) {
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    setColor(COLOR_RED);
    return;
  }
  if (!doc.containsKey(DEVICE_ID)) {
    Serial.print("Could not find schedules for: ");
    Serial.println(DEVICE_ID);
    setColor(COLOR_RED);
    return;
  }
  JsonObject deviceSchedules = doc[DEVICE_ID];
  if (!deviceSchedules.containsKey("on") || !deviceSchedules.containsKey("off")) {
    Serial.print("Could not find on/off schedules in: ");
    Serial.println(deviceSchedules);
    setColor(COLOR_RED);
    return;
  }

  // Release previously stored times, if any
  for(int i = 0; i < onSchedule.size; i++) onSchedule.times[i].clear();
  for(int i = 0; i < offSchedule.size; i++) offSchedule.times[i].clear();
  onSchedule.size = 0;
  offSchedule.size = 0;

  // JsonArray to structs
  JsonArray onArray = deviceSchedules["on"];
  for(JsonVariant v : onArray) {
    String time = v.as<String>();
    onSchedule.times[onSchedule.size++] = time;
    if (onSchedule.size == MAX_TIMES) {
      Serial.print("Reached max schedule times (32).");
      break;
    }
  }
  JsonArray offArray = deviceSchedules["off"];
  for(JsonVariant v : offArray) {
    String time = v.as<String>();
    offSchedule.times[offSchedule.size++] = time;
    if (offSchedule.size == MAX_TIMES) {
      Serial.print("Reached max schedule times (32).");
      break;
    }
  }
  
  // At this point, consider this a successful schedule update
  setColor(COLOR_GREEN);
  sendReadySignal(); // reports only the first time
  sendUpdatedScheduleSignal(); // reports latest update

  // priont to make sure
  for(int i = 0; i < onSchedule.size; i++) {
    Serial.print("on: ");
    Serial.println(onSchedule.times[i]);
  }
  for(int i = 0; i < offSchedule.size; i++) {
    Serial.print("off: ");
    Serial.println(offSchedule.times[i]);
  }
}

void updateServoIfNeeded() {
  if (onSchedule.size == 0 && offSchedule.size == 0) {
    Serial.println("Got no schedule yet, wont update servo settings...");
    return;
  }
  if (!didSyncTime()) {
    Serial.println("Waiting for time to sync...");
    return;
  }

  char currentTimeBuf[42];
  getCurrentTime(currentTimeBuf, sizeof(currentTimeBuf));
  String currentTime = String(currentTimeBuf);

  Serial.print("Current time: ");
  Serial.println(currentTime);

  for(int i = 0; i < onSchedule.size; i++) {
    if (onSchedule.times[i] == currentTime) {
      Serial.println("Match ON schedule!");
      updateServoSetting(DEVICE_ON);
    }
  }
  for(int i = 0; i < offSchedule.size; i++) {
    if (offSchedule.times[i] == currentTime) {
      Serial.println("Match OFF schedule!");
      updateServoSetting(DEVICE_OFF);
    }
  }
  if (lastServoSetting != DEVICE_CENTER && (millis() - lastServoSetTime) > SERVO_MAX_ACTIVATION_TIME) {
    Serial.println("Exceeded max time out of center, recentering...");
    updateServoSetting(DEVICE_CENTER);
  }
}
