#pragma once

#include <WebServer.h>
#include <WiFi.h>

#include "telemetry.h"

class TelemetryTransport {
 public:
  explicit TelemetryTransport(const Telemetry& telemetry);

  void begin();
  void update();

 private:
  const Telemetry& telemetry_;
  WebServer webServer_;
  WiFiServer tcpServer_;
  WiFiClient tcpClient_;
  uint32_t lastSendAt_ = 0;

  void configureRoutes();
  void sendTelemetry();
  void updateTcpClient();
  String buildHtmlPage() const;
};
