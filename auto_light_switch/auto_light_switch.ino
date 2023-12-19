
#include <WiFi.h>
#include <ESP32Servo.h>
#include "secrets.h"

// This device configuration
#define DEVICE_ID     "1"
#define DEVICE_CENTER  90 // TODO Move servo settings to the JSON config
#define DEVICE_ON      70
#define DEVICE_OFF    110
#define SERVO_PIN       2

Servo servo;
unsigned long lastServoSetTime = 0;
unsigned int lastServoSetting = DEVICE_CENTER;

enum {
  COLOR_RED   = 0,
  COLOR_GREEN = 1,
  COLOR_BLUE  = 2,
  COLOR_WHITE = 3
};

void setup() {
  Serial.begin(9600);
  
  // setup WiFi
  Serial.printf("Connecting to %s ", MY_WIFI_SSID);
  WiFi.begin(MY_WIFI_SSID, MY_WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(2000);
      setColor(COLOR_BLUE);
      Serial.print(".");
  }
  Serial.println(" CONNECTED!");

  // setup hardware
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN);
  updateServoSetting(DEVICE_CENTER);
  ledSetup();

  // setup time sync
  syncTime();
}

void loop() {
  ledLoop();
  if (didSyncTime()) {
    configParserLoop();
  }
}

void updateServoSetting(int setting) {
  int safeSetting = constrain(setting, 0, 180);
  Serial.print("Updating servo to setting ");
  Serial.println(safeSetting);
  servo.write(safeSetting);
  lastServoSetting = safeSetting;
  lastServoSetTime = millis();
}
