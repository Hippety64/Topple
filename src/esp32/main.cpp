#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// MAC address: 3C:71:BF:9D:CB:E0

// ESP-01S MAC address (receiver)
uint8_t receiverMac[] = {0x34, 0x94, 0x54, 0x85, 0x27, 0x65};

// Simple protocol: just send a blink command
typedef struct {
  uint8_t command;  // 1 = blink
} BlinkMessage;

esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32 Sender Ready");

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(onDataSent);

  // Register peer
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Peer registered successfully");
}

void loop() {
  BlinkMessage msg;
  msg.command = 1;  // Blink command

  // Send message
  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&msg, sizeof(msg));

  if (result == ESP_OK) {
    Serial.println("Blink command sent");
  } else {
    Serial.println("Failed to send blink command");
  }

  delay(1000);  // Send once per second
}
