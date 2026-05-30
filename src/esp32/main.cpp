#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32 ready");
}

void loop() {
  delay(1000);
}
