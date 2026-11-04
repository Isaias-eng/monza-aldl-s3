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
| `src/main.cpp` | Inicialização e loop principal |
| `src/config.h` | Pinos, portas, tempos e credenciais |
| `src/telemetry.h/.cpp` | Modelo e serialização dos dados |
| `src/aldl_reader.h/.cpp` | UART, sincronização, checksum e decodificação ALDL |
| `src/display_manager.h/.cpp` | Inicialização e atualização do OLED |
| `src/telemetry_transport.h/.cpp` | USB, Wi-Fi, HTTP, JSON e TCP |

As classes recebem suas dependências por referência, evitando variáveis globais espalhadas e tornando cada subsistema isolado para futuras alterações.

## Dependências

O projeto usa **PlatformIO** no VS Code.

Instale a extensão **PlatformIO IDE** e abra a pasta do projeto:

```text
/home/isaias/Documents/monza-aldl-s3
```

As dependências são instaladas automaticamente a partir do `platformio.ini`:

- Adafruit GFX Library
- Adafruit SSD1306

O pacote da plataforma ESP32 também será instalado automaticamente. A configuração atual usa `esp32-s3-devkitc-1`, que é adequada para placas de desenvolvimento baseadas no ESP32-S3 WROOM-1. Se a sua placa possuir outro pinout, altere `board` no `platformio.ini`.

## Compilar e gravar

No terminal do VS Code:

```bash
pio run
pio run --target upload
pio device monitor
```

Ou use os botões de **Build**, **Upload** e **Monitor** da barra do PlatformIO.

O monitor serial usa 115200 baud.

## Ligações padrão

Os pinos estão em `src/config.h`:

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
