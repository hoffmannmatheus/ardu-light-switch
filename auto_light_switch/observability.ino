
#include <HTTPClient.h>
#include <ArduinoJson.h>


enum HTTP_METHOD {
  POST,
  PUT,
  PATCH
};

bool didSendFirstReadySignal = false;

void sendReadySignal() {
  if (didSendFirstReadySignal) {
    Serial.println("[observability] Already sent booted and ready signal.");
    return;
  }

  char currentTimeBuf[42];
  getCurrentIsoTime(currentTimeBuf, sizeof(currentTimeBuf));
  String currentTime = String(currentTimeBuf);

  String path = String(FIREBASE_DB_BASE_URL)
    + "/observability/booted_and_ready/" + String(DEVICE_ID) + "/"
    + String(currentTime) + ".json?auth=" + String(FIREBASE_DB_AUTH_KEY);
  String payload = "{\"status\": \"ready\"}";

  if (sendSignal(PUT, path, payload)) {
    Serial.println("[observability] Successfully sent booted and ready signal!");
    didSendFirstReadySignal = true;
  }
}

void sendUpdatedScheduleSignal() {
  char currentTimeBuf[42];
  getCurrentIsoTime(currentTimeBuf, sizeof(currentTimeBuf));
  String currentTime = String(currentTimeBuf);

  String path = String(FIREBASE_DB_BASE_URL)
    + "/observability/last_schedule_update/.json?auth="
    + String(FIREBASE_DB_AUTH_KEY);
  String payload = "{\"" + String(DEVICE_ID) + "\": \""
    + String(currentTime) + "\"}";

  if (sendSignal(PATCH, path, payload)) {
    Serial.println("[observability] Successfully sent schedule update signal!");
    didSendFirstReadySignal = true;
  }
}

bool sendSignal(int method, String path, String payload) {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  HTTPClient http;
  int code;
  http.begin(path.c_str());

  switch (method) {
    case POST:
      code = http.POST(payload);
      break;
    case PUT:
      code = http.PUT(payload);
      break;
    case PATCH:
      code = http.PATCH(payload);
      break;
    default:
      http.end();
      return false;
  }

  Serial.print("[observability] HTTP PUT response: ");
  Serial.println(code);

  http.end();
  return code == 200;
}
