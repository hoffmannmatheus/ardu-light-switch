
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define CONFIG_POLL_INTERVAL  600000 // 10 minutes
#define SERVO_UPDATE_INTERVAL 500
#define CONFIG_URL "https://raw.githubusercontent.com/hoffmannmatheus/ardu-light-switch/main/schedules/daily"

bool didPollFirstLoop = false;
unsigned long lastConfigPollTime = 0;
unsigned long lastServoUpdateCheckTime = 0;
unsigned long lastServoSetTime = 0;
char lastServoSetTime[42];
JsonObject schedule;

void configParserLoop() {
  unsigned long now = millis();
  if ((now - lastConfigPollTime > CONFIG_POLL_INTERVAL) || !didPollFirstLoop) {
    lastConfigPollTime = now;
    getRemoteConfig();
  };
  if (now - lastServoUpdateCheckTime > SERVO_UPDATE_INTERVAL) {
    lastServoUpdateCheckTime = now;
    updateServoIfNeeded();
  }
}

void getRemoteConfig() {
  didPollFirstLoop = true;
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected, maybe next time!");
    return;
  }
  Serial.println("Will poll configuration...");

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
    Serial.println(payload);
    handleConfigurationPayload(payload);
    break;
  default:
    Serial.println("Error...");
    break;
  }

  http.end();
}

void handleConfigurationPayload(String payload) {
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  if (!doc.containsKey(DEVICE_ID)) {
    Serial.print("Could not find schedules for: ");
    Serial.println(DEVICE_ID);
    return;
  }
  schedule = doc[DEVICE_ID];
}

void updateServoIfNeeded() {
  if (!schedule) {
    Serial.println("Got no schedule yet, wont update servo settings...");
    return;
  }
  if (!didSyncTime()) {
    Serial.println("Waiting for time to sync...");
    return;
  }

  char currentTime[42];
  getCurrentTime(currentTime, sizeof(currentTime));

  Serial.print("Current time: ");
  Serial.println(String(currentTime));

  if (schedule.containsKey(currentTime)) {
    lastServoSetTime = millis();
    updateServoSetting(schedule[currentTime]);
  }
}
