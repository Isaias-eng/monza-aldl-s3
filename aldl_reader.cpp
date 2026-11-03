#include "aldl_reader.h"

#include "config.h"

const uint8_t AldlReader::REQUEST_SHUTUP[4] = {0xF4, 0x56, 0x08, 0xAE};
const uint8_t AldlReader::REQUEST_STREAM[5] = {0xF4, 0x57, 0x01, 0x00, 0xB4};
const uint8_t AldlReader::FRAME_START[3] = {0xF4, 0x95, 0x01};

AldlReader::AldlReader(Telemetry& telemetry) : telemetry_(telemetry) {}

void AldlReader::begin() {
  Serial2.begin(
      config::ALDL_BAUD_RATE,
      SERIAL_8N1,
      config::ALDL_RX_PIN,
      config::ALDL_TX_PIN,
      false);

  while (Serial2.available()) {
    Serial2.read();
  }

  rxLength_ = 0;
  startedAt_ = millis();
  lastPollAt_ = 0;
  send(REQUEST_SHUTUP, sizeof(REQUEST_SHUTUP));
  shutupSent_ = true;
}

void AldlReader::update() {
  readSerial();
  processBuffer();

  const uint32_t now = millis();
  if (shutupSent_ &&
      now - startedAt_ >= config::ALDL_START_DELAY_MS &&
      now - lastPollAt_ >= config::ALDL_POLL_INTERVAL_MS) {
    lastPollAt_ = now;
    send(REQUEST_STREAM, sizeof(REQUEST_STREAM));
  }

  telemetry_.ecuOnline =
      telemetry_.lastFrameAt != 0 &&
      now - telemetry_.lastFrameAt < config::ALDL_TIMEOUT_MS;
}

void AldlReader::readSerial() {
  while (Serial2.available() > 0) {
    if (rxLength_ >= sizeof(rxBuffer_)) {
      rxLength_ = 0;
      break;
    }
    rxBuffer_[rxLength_++] = static_cast<uint8_t>(Serial2.read());
  }
}

void AldlReader::processBuffer() {
  while (rxLength_ > 0) {
    const int start = findFrameStart();
    if (start < 0) {
      if (rxLength_ > sizeof(FRAME_START)) {
        rxLength_ = 0;
      }
      return;
    }

    if (start > 0) {
      removePrefix(static_cast<size_t>(start));
      continue;
    }

    if (rxLength_ < FRAME_LENGTH) {
      return;
    }

    uint8_t frame[FRAME_LENGTH];
    memcpy(frame, rxBuffer_, FRAME_LENGTH);
    if (!validChecksum(frame, FRAME_LENGTH)) {
      ++telemetry_.checksumErrors;
      removePrefix(1);
      continue;
    }

    decodeFrame(frame);
    removePrefix(FRAME_LENGTH);
  }
}

void AldlReader::decodeFrame(const uint8_t* frame) {
  const uint8_t rpmRaw = payloadByte(frame, 10);
  const uint8_t tpsRaw = payloadByte(frame, 9);
  const uint8_t ctsRaw = payloadByte(frame, 6);
  const uint8_t iatRaw = payloadByte(frame, 30);
  const uint8_t speedRaw = payloadByte(frame, 13);
  const uint8_t iacRaw = payloadByte(frame, 21);
  const uint8_t mapRaw = payloadByte(frame, 26);
  const uint8_t vbatRaw = payloadByte(frame, 32);
  const uint8_t co2Raw = payloadByte(frame, 16);
  const uint8_t sparkRaw = payloadByte(frame, 33);
  const uint8_t afrRaw = payloadByte(frame, 38);
  const uint16_t injectionRaw =
      (static_cast<uint16_t>(payloadByte(frame, 36)) << 8) |
      payloadByte(frame, 37);

  telemetry_.ecuId =
      (static_cast<uint16_t>(payloadByte(frame, 1)) << 8) |
      payloadByte(frame, 2);
  telemetry_.malfunction1 = payloadByte(frame, 3);
  telemetry_.malfunction2 = payloadByte(frame, 4);
  telemetry_.malfunction3 = payloadByte(frame, 5);
  telemetry_.rpm = static_cast<int>(rpmRaw * 25.0f);
  telemetry_.tps = tpsRaw / 2.55f;
  telemetry_.cts = ctsRaw * 0.75f - 40.0f;
  telemetry_.iat = iatRaw * 0.75f - 40.0f;
  telemetry_.speed = speedRaw;
  telemetry_.iac = iacRaw;
  telemetry_.map = mapRaw * (5.0f / 255.0f);
  telemetry_.vbat = vbatRaw / 10.0f;
  telemetry_.co2Voltage = co2Raw * (5.0f / 255.0f);
  telemetry_.sparkAdvance = sparkRaw * 90.0f / 256.0f;
  telemetry_.afr = afrRaw / 10.0f;
  telemetry_.injectionMs = injectionRaw / 65.536f;
  ++telemetry_.packets;
  telemetry_.lastFrameAt = millis();
  telemetry_.ecuOnline = true;
}

void AldlReader::send(const uint8_t* bytes, size_t length) {
  Serial2.write(bytes, length);
  Serial2.flush();
}

int AldlReader::findFrameStart() const {
  for (size_t i = 0; i + sizeof(FRAME_START) <= rxLength_; ++i) {
    bool matches = true;
    for (size_t j = 0; j < sizeof(FRAME_START); ++j) {
      if (rxBuffer_[i + j] != FRAME_START[j]) {
        matches = false;
        break;
      }
    }
    if (matches) return static_cast<int>(i);
  }
  return -1;
}

void AldlReader::removePrefix(size_t count) {
  if (count == 0 || count > rxLength_) return;
  const size_t remaining = rxLength_ - count;
  if (remaining > 0) {
    memmove(rxBuffer_, rxBuffer_ + count, remaining);
  }
  rxLength_ = remaining;
}

uint8_t AldlReader::payloadByte(const uint8_t* frame, size_t index) {
  return frame[HEADER_LENGTH + index - 1];
}

bool AldlReader::validChecksum(const uint8_t* frame, size_t length) {
  uint32_t sum = 0;
  for (size_t i = 0; i < length; ++i) {
    sum += frame[i];
  }
  return (sum & 0xFF) == 0;
}
