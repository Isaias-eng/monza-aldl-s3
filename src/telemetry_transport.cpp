#include "telemetry_transport.h"

#include "config.h"

TelemetryTransport::TelemetryTransport(const Telemetry& telemetry)
    : telemetry_(telemetry),
      webServer_(config::HTTP_PORT),
      tcpServer_(config::TCP_PORT) {}

void TelemetryTransport::begin() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(config::WIFI_SSID, config::WIFI_PASSWORD);

  configureRoutes();
  webServer_.begin();
  tcpServer_.begin();
}

void TelemetryTransport::update() {
  updateTcpClient();
  webServer_.handleClient();

  if (millis() - lastSendAt_ >= config::TELEMETRY_INTERVAL_MS) {
    lastSendAt_ = millis();
    sendTelemetry();
  }
}

void TelemetryTransport::configureRoutes() {
  webServer_.on("/", HTTP_GET, [this]() {
    webServer_.send(
        200,
        "text/html; charset=utf-8",
        buildHtmlPage());
  });

  webServer_.on("/api/data", HTTP_GET, [this]() {
    String json;
    serializeTelemetry(telemetry_, json);
    webServer_.send(200, "application/json", json);
  });
}

void TelemetryTransport::sendTelemetry() {
  String json;
  serializeTelemetry(telemetry_, json);
  Serial.println(json);

  if (tcpClient_ && tcpClient_.connected()) {
    tcpClient_.println(json);
  }
}

void TelemetryTransport::updateTcpClient() {
  if (!tcpClient_ || !tcpClient_.connected()) {
    WiFiClient candidate = tcpServer_.available();
    if (candidate) {
      tcpClient_ = candidate;
    }
  }

  while (tcpClient_ && tcpClient_.available()) {
    String command = tcpClient_.readStringUntil('\n');
    command.trim();
    if (command == "PING") {
      tcpClient_.println("PONG");
    }
  }
}

String TelemetryTransport::buildHtmlPage() const {
  return R"HTML(<!doctype html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width">
<title>Monza ALDL</title>
<style>
body{font-family:Arial;background:#101318;color:#eee;margin:20px}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(130px,1fr));gap:10px}
.card{background:#1d2630;padding:12px;border-radius:8px}
.v{font-size:1.5em;color:#66d9ef}
</style>
</head>
<body>
<h1>Monza ALDL</h1>
<p id="status">Conectando...</p>
<div class="grid" id="data"></div>
<script>
const fields=[
  ['rpm','RPM'],['cts','CTS °C'],['iat','IAT °C'],['tps','TPS %'],
  ['map','MAP V'],['vbat','VBAT V'],['vel','Vel km/h'],['iac','IAC'],
  ['inj','Inj ms'],['spk','SPK °'],['afr','AFR']
];
function draw(d){
  status.textContent=d.ecu?'ECU online':'Aguardando ECU';
  data.innerHTML=fields.map(x =>
    `<div class="card">${x[1]}<div class="v">${d[x[0]]}</div></div>`
  ).join('');
}
async function poll(){
  try { draw(await (await fetch('/api/data')).json()); }
  catch (e) { status.textContent='Sem conexão'; }
}
setInterval(poll,500);
poll();
</script>
</body>
</html>)HTML";
}
