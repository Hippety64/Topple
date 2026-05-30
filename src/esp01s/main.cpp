#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP-01S ready");
}

void loop() {
  delay(1000);
}
