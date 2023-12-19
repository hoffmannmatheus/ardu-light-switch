
#include <WiFi.h>
#include <ESP32Servo.h>
#include "secrets.h"

#define SERVO_PIN 2
#define DEVICE_ID "2"
#define DEVICE_CENTER 96

Servo servo;

void setup() {
  Serial.begin(9600);
  
  // setup WiFi
  Serial.printf("Connecting to %s ", MY_WIFI_SSID);
  WiFi.begin(MY_WIFI_SSID, MY_WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(2000);
      Serial.print(".");
  }
  Serial.println(" CONNECTED!");

  // setup servo
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN);
  servo.write(DEVICE_CENTER);

  // setup time sync
  syncTime();
}

void loop() {
  if (didSyncTime()) {
    configParserLoop();
  }
}

void updateServoSetting(int setting) {
  int safeSetting = constrain(setting, 0, 180);
  Serial.print("Updating servo to setting ");
  Serial.println(safeSetting);
  servo.write(safeSetting);
}
