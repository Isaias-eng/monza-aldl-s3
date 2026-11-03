#pragma once

#include <Arduino.h>

#include "telemetry.h"

class AldlReader {
 public:
  explicit AldlReader(Telemetry& telemetry);

  void begin();
  void update();

 private:
  static constexpr size_t HEADER_LENGTH = 3;
  static constexpr size_t PAYLOAD_LENGTH = 63;
  static constexpr size_t FRAME_LENGTH = HEADER_LENGTH + PAYLOAD_LENGTH + 1;
  static constexpr size_t RX_BUFFER_LENGTH = 512;

  static const uint8_t REQUEST_SHUTUP[4];
  static const uint8_t REQUEST_STREAM[5];
  static const uint8_t FRAME_START[3];

  Telemetry& telemetry_;
  uint8_t rxBuffer_[RX_BUFFER_LENGTH] = {};
  size_t rxLength_ = 0;
  uint32_t startedAt_ = 0;
  uint32_t lastPollAt_ = 0;
  bool shutupSent_ = false;

  void readSerial();
  void processBuffer();
  void decodeFrame(const uint8_t* frame);
  void send(const uint8_t* bytes, size_t length);

  int findFrameStart() const;
  void removePrefix(size_t count);
  static uint8_t payloadByte(const uint8_t* frame, size_t index);
  static bool validChecksum(const uint8_t* frame, size_t length);
};
