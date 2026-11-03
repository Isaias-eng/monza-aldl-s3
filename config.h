#pragma once

#include <Arduino.h>

namespace config {

// Ajuste estes pinos conforme a sua placa de desenvolvimento.
constexpr int ALDL_RX_PIN = 16;
constexpr int ALDL_TX_PIN = 17;
constexpr int OLED_SDA_PIN = 8;
constexpr int OLED_SCL_PIN = 9;
constexpr int BUZZER_PIN = 33;

constexpr uint32_t USB_BAUD_RATE = 115200;
constexpr uint32_t ALDL_BAUD_RATE = 8192;
constexpr uint32_t ALDL_POLL_INTERVAL_MS = 100;
constexpr uint32_t ALDL_START_DELAY_MS = 500;
constexpr uint32_t ALDL_TIMEOUT_MS = 2000;

constexpr uint32_t TELEMETRY_INTERVAL_MS = 200;
constexpr uint32_t OLED_UPDATE_INTERVAL_MS = 500;

constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

constexpr uint16_t HTTP_PORT = 80;
constexpr uint16_t TCP_PORT = 3333;
constexpr char WIFI_SSID[] = "MonzaALDL-S3";
constexpr char WIFI_PASSWORD[] = "12345678";

}  // namespace config
