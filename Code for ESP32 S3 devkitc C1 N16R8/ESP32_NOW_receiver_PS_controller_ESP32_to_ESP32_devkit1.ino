#include <WiFi.h>
#include <esp_now.h>

typedef struct __attribute__((packed)) {
  int16_t lx;
  int16_t ly;
  int16_t rx;
  int16_t ry;
  uint16_t L2;
  uint16_t R2;
  uint32_t buttons;
  uint8_t  dpad;
  uint8_t  misc;
  // uint8_t  seq;
} ControllerPacket;

ControllerPacket rxData;
volatile unsigned long lastRxTime = 0;

// ✅ v3.x callback signature
void onDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData,
                int len) {

  if (len == sizeof(ControllerPacket)) {
    memcpy(&rxData, incomingData, sizeof(rxData));
    lastRxTime = millis();

    // Serial.print("From MAC: ");
    // for (int i = 0; i < 6; i++) {
    //   Serial.printf("%02X", info->src_addr[i]);
    //   if (i < 5) Serial.print(":");
    // }
    // Serial.println();

    Serial.print("LX: "); Serial.print(rxData.lx);
    Serial.print(" LY: "); Serial.print(rxData.ly);
    Serial.print(" RX: "); Serial.print(rxData.rx);
    Serial.print(" RY: "); Serial.print(rxData.ry);
    Serial.print("L2: "); Serial.print(rxData.L2);
    Serial.print(" R2: "); Serial.print(rxData.R2);
    Serial.print(" buttons: "); Serial.print(rxData.buttons, HEX);
    Serial.print(" dpad: "); Serial.print(rxData.dpad, HEX);
    Serial.print(" misc: "); Serial.println(rxData.misc, HEX);
    // Serial.print(" BTN: "); Serial.println(rxData.buttons, HEX);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (true);
  }

  else {
    Serial.println("ESP-NOW init success");
  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.println("ESP32-S3 ESP-NOW Receiver (v3.x) Ready");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  if (millis() - lastRxTime > 300) {
    // failsafe
  }
}
