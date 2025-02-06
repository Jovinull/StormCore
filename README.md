# Estação Meteorológica com ESP32

Este projeto implementa uma estação meteorológica utilizando um ESP32 e sensores para medir temperatura, umidade, pressão atmosférica, velocidade e direção do vento, além de pluviosidade. Os dados coletados são enviados para um broker MQTT (HiveMQ).

## 🛠️ Componentes Utilizados
- **ESP32**
- **Sensor de temperatura e umidade**: DHT22
- **Sensor de pressão atmosférica**: BMP180
- **Sensor de velocidade do vento**
- **Sensor de direção do vento**
- **Sensor de chuva**
- **Broker MQTT**: HiveMQ

## 📡 Funcionalidades
- Conexão à rede Wi-Fi.
- Publicação de dados meteorológicos via MQTT.
- Medição de:
  - Temperatura (°C)
  - Umidade relativa (%)
  - Pressão atmosférica (hPa)
  - Velocidade do vento (km/h)
  - Direção do vento (N, S, L, O e combinações)
  - Pluviosidade (mm de chuva)

## 🔧 Configuração e Uso

### 1️⃣ Instalação das Bibliotecas Necessárias
Antes de compilar e carregar o código no ESP32, instale as seguintes bibliotecas na IDE Arduino:
- `WiFi.h`
- `PubSubClient.h`
- `Adafruit_BMP085.h`
- `DHT.h`

Essas bibliotecas podem ser encontradas no gerenciador de bibliotecas da IDE Arduino.

### 2️⃣ Configuração do Código
Edite o código para incluir as credenciais de Wi-Fi:
```cpp
const char* ssid = "SEU_WIFI_SSID";
const char* password = "SUA_WIFI_SENHA";
```

Caso utilize um broker MQTT diferente do HiveMQ, altere:
```cpp
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
```

### 3️⃣ Upload e Execução
- Conecte o ESP32 ao computador via USB.
- Compile e envie o código.
- Monitore os dados através do **Serial Monitor** (115200 baud).
- Utilize um cliente MQTT para visualizar os dados publicados nos tópicos:
  - `weather/temperature`
  - `weather/humidity`
  - `weather/pressure`
  - `weather/wind_speed`
  - `weather/wind_direction`
  - `weather/rain`

## 📝 Explicação do Código
### 📡 Conexão Wi-Fi e MQTT
O ESP32 conecta-se à rede Wi-Fi e ao broker MQTT para publicar os dados dos sensores.

### 🌡️ Medição dos Sensores
- O sensor DHT22 mede temperatura e umidade.
- O sensor BMP180 mede pressão atmosférica.
- Sensores de vento capturam velocidade e direção.
- Um pluviômetro registra a quantidade de chuva.

### 📤 Publicação de Dados
A cada 5 segundos, os dados são lidos e publicados via MQTT.

## 📌 Melhorias Futuras
- Integração com banco de dados para armazenamento dos dados coletados.
- Interface web para visualização dos dados.
- Adição de sensores adicionais, como UV e qualidade do ar.

## 📬 Contato
Caso tenha dúvidas ou sugestões, entre em contato!
