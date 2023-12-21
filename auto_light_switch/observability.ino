
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

  if (sendSignal(path, payload)) {
    Serial.println("[observability] Successfully sent booted and ready signal!");
    didSendFirstReadySignal = true;
  } else {
    Serial.println("[observability] Did not successfully send booted and ready signal");
  }
}

bool sendSignal(String path, String payload) {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  HTTPClient http;
  http.begin(path.c_str());
  int code = http.PUT(payload);

  Serial.print("[observability] HTTP PUT response: ");
  Serial.println(code);

  http.end();
  return code == 200;
}
