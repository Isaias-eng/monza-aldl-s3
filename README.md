# Monza ALDL S3

Projeto inicial simplificado para:

- ESP32-S3 WROOM-1;
- ECU Multec 700 usando a interface ALDL do projeto base;
- OLED SSD1306 I2C 128x64;
- buzzer passivo;
- telemetria via USB serial e Wi-Fi.

## Organização do código

O firmware foi dividido por responsabilidade:

| Arquivo | Responsabilidade |
|---|---|
| `monza-aldl-s3.ino` | Inicialização e loop principal |
| `config.h` | Pinos, portas, tempos e credenciais |
| `telemetry.h/.cpp` | Modelo e serialização dos dados |
| `aldl_reader.h/.cpp` | UART, sincronização, checksum e decodificação ALDL |
| `display_manager.h/.cpp` | Inicialização e atualização do OLED |
| `telemetry_transport.h/.cpp` | USB, Wi-Fi, HTTP, JSON e TCP |

As classes recebem suas dependências por referência, evitando variáveis globais espalhadas e tornando cada subsistema isolado para futuras alterações.

## Dependências

Instale pela Arduino IDE:

- Adafruit GFX Library
- Adafruit SSD1306

Selecione uma placa ESP32-S3 compatível com o seu módulo. O projeto usa `Serial` USB a 115200 baud.

## Ligações padrão

Os pinos estão em `config.h`:

| Função | GPIO padrão |
|---|---:|
| ALDL RX | 16 |
| ALDL TX | 17 |
| OLED SDA | 8 |
| OLED SCL | 9 |
| Buzzer | 33 |

Confirme os GPIOs na sua placa de desenvolvimento antes de ligar o circuito. A linha ALDL não deve ser ligada diretamente ao ESP32; use a interface elétrica do projeto base.

## USB

Abra o monitor serial em 115200 baud. O ESP32 envia uma linha JSON a cada 200 ms:

```json
{"t":1234,"ecu":true,"rpm":900,"cts":85.0,"vbat":13.8}
```

## Wi-Fi

O ESP32 cria:

- SSID: `MonzaALDL-S3`
- Senha: `12345678`
- Página: `http://192.168.4.1/`
- JSON: `http://192.168.4.1/api/data`
- TCP: `192.168.4.1:3333`

O TCP também envia uma linha JSON a cada 200 ms. O comando `PING\n` responde `PONG`.

## Próximos passos

1. Confirmar os GPIOs da placa física.
2. Testar OLED e USB sem a ECU.
3. Testar a interface ALDL em bancada.
4. Comparar os valores decodificados com o firmware original.
5. Adicionar telas alternadas no OLED e alertas do buzzer.

## Observações de segurança

- A linha ALDL não deve ser ligada diretamente ao ESP32; use a interface elétrica do projeto base.
- Altere a senha do Access Point antes de usar o equipamento em um ambiente compartilhado.
- O firmware atual é um protótipo de monitoramento e ainda não implementa autenticação na página HTTP ou no TCP.
