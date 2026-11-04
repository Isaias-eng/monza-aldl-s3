#include "telemetry.h"

void serializeTelemetry(const Telemetry& telemetry, String& output) {
  output = "{";
  output += "\"t\":" + String(millis());
  output += ",\"ecu\":" + String(telemetry.ecuOnline ? "true" : "false");
  output += ",\"id\":" + String(telemetry.ecuId);
  output += ",\"rpm\":" + String(telemetry.rpm);
  output += ",\"tps\":" + String(telemetry.tps, 2);
  output += ",\"map\":" + String(telemetry.map, 2);
  output += ",\"cts\":" + String(telemetry.cts, 1);
  output += ",\"iat\":" + String(telemetry.iat, 1);
  output += ",\"vbat\":" + String(telemetry.vbat, 2);
  output += ",\"vel\":" + String(telemetry.speed);
  output += ",\"iac\":" + String(telemetry.iac);
  output += ",\"inj\":" + String(telemetry.injectionMs, 2);
  output += ",\"co2\":" + String(telemetry.co2Voltage, 2);
  output += ",\"spk\":" + String(telemetry.sparkAdvance, 1);
  output += ",\"afr\":" + String(telemetry.afr, 1);
  output += ",\"m1\":" + String(telemetry.malfunction1);
  output += ",\"m2\":" + String(telemetry.malfunction2);
  output += ",\"m3\":" + String(telemetry.malfunction3);
  output += ",\"pkt\":" + String(telemetry.packets);
  output += ",\"cks\":" + String(telemetry.checksumErrors);
  output += "}";
}
