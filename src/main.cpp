#include "aldl_reader.h"
#include "config.h"
#include "display_manager.h"
#include "telemetry_transport.h"

Telemetry telemetry;
AldlReader aldlReader(telemetry);
DisplayManager displayManager(telemetry);
TelemetryTransport transport(telemetry);

void setup() {
  Serial.begin(config::USB_BAUD_RATE);
  delay(100);

  displayManager.begin();
  transport.begin();
  aldlReader.begin();
}

void loop() {
  aldlReader.update();
  transport.update();
  displayManager.update();
}
