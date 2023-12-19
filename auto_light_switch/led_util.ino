/*
  Arduino led utilities.
*/

#define LED_BLINK_INTERVAL 1000
#define LED_NO_COLOR         -1

unsigned int lastSetColor;
unsigned long lastLedLoopTime = 0;
bool blinkOn = true;

void ledSetup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  setColor(LED_NO_COLOR);
}

void ledLoop() {
  unsigned long now = millis();
  if (now - lastLedLoopTime > LED_BLINK_INTERVAL) {
    lastLedLoopTime = now;
    if (blinkOn) {
      setColor(lastSetColor);
    } else {
      setColor(LED_NO_COLOR);
    }
    blinkOn = !blinkOn;
  }
}

void setColor(int color) {
  if (color != LED_NO_COLOR) {
    lastSetColor = color;
  }
  switch (color) {
    case COLOR_RED:
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, HIGH);
      break;
    case COLOR_GREEN:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
      break;
    case COLOR_BLUE:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
      break;
    case COLOR_WHITE:
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
      break;
    default:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, HIGH);
      break;
  }      
}