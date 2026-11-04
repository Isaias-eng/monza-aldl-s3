#pragma once

#include <Adafruit_SSD1306.h>

#include "telemetry.h"

class DisplayManager {
 public:
  explicit DisplayManager(const Telemetry& telemetry);

  void begin();
  void update();

 private:
  const Telemetry& telemetry_;
  Adafruit_SSD1306 display_;
  bool ready_ = false;
  uint32_t lastUpdateAt_ = 0;

  void drawStartupScreen();
  void drawTelemetry();
};
