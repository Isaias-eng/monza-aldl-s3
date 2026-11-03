#include "display_manager.h"

#include <Wire.h>

#include "config.h"

DisplayManager::DisplayManager(const Telemetry& telemetry)
    : telemetry_(telemetry),
      display_(config::OLED_WIDTH, config::OLED_HEIGHT, &Wire, -1) {}

void DisplayManager::begin() {
  pinMode(config::BUZZER_PIN, OUTPUT);
  noTone(config::BUZZER_PIN);

  Wire.begin(config::OLED_SDA_PIN, config::OLED_SCL_PIN);
  ready_ = display_.begin(
      SSD1306_SWITCHCAPVCC,
      config::OLED_ADDRESS);

  if (ready_) {
    drawStartupScreen();
  }
}

void DisplayManager::update() {
  if (!ready_ ||
      millis() - lastUpdateAt_ < config::OLED_UPDATE_INTERVAL_MS) {
    return;
  }

  lastUpdateAt_ = millis();
  drawTelemetry();
}

void DisplayManager::drawStartupScreen() {
  display_.clearDisplay();
  display_.setTextColor(SSD1306_WHITE);
  display_.setTextSize(1);
  display_.setCursor(0, 0);
  display_.println("MONZA ALDL S3");
  display_.println("Inicializando...");
  display_.display();
}

void DisplayManager::drawTelemetry() {
  display_.clearDisplay();
  display_.setTextColor(SSD1306_WHITE);
  display_.setTextSize(1);
  display_.setCursor(0, 0);
  display_.println(telemetry_.ecuOnline ? "ECU ONLINE" : "AGUARDANDO ECU");

  display_.setCursor(0, 12);
  display_.setTextSize(2);
  display_.print(telemetry_.rpm);
  display_.println(" RPM");

  display_.setTextSize(1);
  display_.setCursor(0, 34);
  display_.printf(
      "CTS %5.1f C  IAT %5.1f C",
      telemetry_.cts,
      telemetry_.iat);
  display_.setCursor(0, 45);
  display_.printf(
      "TPS %4.1f%%  MAP %4.2fV",
      telemetry_.tps,
      telemetry_.map);
  display_.setCursor(0, 56);
  display_.printf(
      "BAT %4.1fV P:%lu E:%lu",
      telemetry_.vbat,
      telemetry_.packets,
      telemetry_.checksumErrors);
  display_.display();
}
