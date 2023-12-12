/*
  Servo calibrator tool.

  This doesn't actually change anything in the servos, but lets you enter servo angle values 
  via Serial so that you can know which values to use for certian applications. For example, 
  This allowed me to easly find I needed 70 & 110 values for my light swith to turn On/Off.
*/

#include <ESP32Servo.h>

#define SERVO_PIN 2

Servo servo;
int servoPosition = 90;  // center

void setup() {
  Serial.begin(115200);
  delay(1000);

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN);
  
  Serial.print("Manual servo calibrator. Please type INT values and press ENTER. Current value: ");
  Serial.println(servoPosition);
}

void loop() {
  servo.write(servoPosition);

  while (Serial.available() > 0) {
    int typedInt = Serial.parseInt();
    if (Serial.read() == '\n') {
      servoPosition = constrain(typedInt, 0, 180);
      Serial.print("Setting to: ");
      Serial.println(servoPosition);
      
      servo.write(servoPosition);
    }
  }
}
