#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>

// ESP-01S onboard LED is on GPIO2
#define LED_PIN 2

// Simple protocol: just receive blink command
typedef struct {
  uint8_t command;  // 1 = blink
} BlinkMessage;

volatile bool blinkRequested = false;

// Callback when data is received
void onDataReceived(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len < sizeof(BlinkMessage)) {
    return;
  }

  BlinkMessage msg;
  memcpy(&msg, incomingData, sizeof(msg));

  if (msg.command == 1) {
    blinkRequested = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print("ESP-01S MAC: ");
  Serial.println(WiFi.macAddress());
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("ESP-01S Receiver Ready");

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceived);

  Serial.println("ESP-NOW initialized as receiver");
}

void loop() {
  if (blinkRequested) {
    blinkRequested = false;
    Serial.println("Blink command received!");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }

  delay(100);
}
