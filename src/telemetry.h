#pragma once

#include <Arduino.h>

struct Telemetry {
  uint16_t ecuId = 0;
  uint8_t malfunction1 = 0;
  uint8_t malfunction2 = 0;
  uint8_t malfunction3 = 0;
  int rpm = 0;
  float tps = 0;
  float map = 0;
  float cts = 0;
  float iat = 0;
  float vbat = 0;
  int speed = 0;
  int iac = 0;
  float injectionMs = 0;
  float co2Voltage = 0;
  float sparkAdvance = 0;
  float afr = 0;
  uint32_t packets = 0;
  uint32_t checksumErrors = 0;
  bool ecuOnline = false;
  uint32_t lastFrameAt = 0;
};

void serializeTelemetry(const Telemetry& telemetry, String& output);
