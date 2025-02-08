#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SHT31.h>

// Configurações do Wi-Fi
const char* ssid = "SEU_WIFI_SSID";
const char* password = "SUA_WIFI_SENHA";

// Configuração do MQTT (HiveMQ)
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic_temp = "weather/temperature";
const char* topic_hum = "weather/humidity";
const char* topic_pressure = "weather/pressure";
const char* topic_wind_speed = "weather/wind_speed";
const char* topic_wind_direction = "weather/wind_direction";
const char* topic_rain = "weather/rain";

WiFiClient espClient;
PubSubClient client(espClient);

// Sensores
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;

// Pinos dos sensores de vento e chuva
#define WIND_SPEED_PIN 19
#define WIND_DIR_N 33
#define WIND_DIR_W 25
#define WIND_DIR_S 26
#define WIND_DIR_E 27
#define RAIN_SENSOR_PIN 23

volatile int wind_count = 0;
volatile int rain_count = 0;

// Função para conectar ao Wi-Fi
void setup_wifi() {
    Serial.print("Conectando ao WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println(" Conectado!");
}

// Função para reconectar ao MQTT
void reconnect() {
    while (!client.connected()) {
        Serial.print("Conectando ao MQTT...");
        if (client.connect("ESP32WeatherStation")) {
            Serial.println(" Conectado!");
        } else {
            Serial.print("Falha, código: ");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5s...");
            delay(5000);
        }
    }
}

// Função para capturar o vento
void IRAM_ATTR countWind() {
    wind_count++;
}

// Função para capturar a chuva
void IRAM_ATTR countRain() {
    rain_count++;
}

// Função para obter a direção do vento
String getWindDirection() {
    bool north = !digitalRead(WIND_DIR_N);
    bool west = !digitalRead(WIND_DIR_W);
    bool south = !digitalRead(WIND_DIR_S);
    bool east = !digitalRead(WIND_DIR_E);

    if (north && west) return "North-West";
    if (north && east) return "North-East";
    if (south && west) return "South-West";
    if (south && east) return "South-East";
    if (north) return "North";
    if (east) return "East";
    if (south) return "South";
    if (west) return "West";
    
    return "Unknown";
}

void setup() {
    Serial.begin(115200);
    
    // Conectar WiFi
    setup_wifi();

    // Configurar MQTT
    client.setServer(mqtt_server, mqtt_port);

    // Inicializar Sensores
    if (!sht31.begin(0x44)) {
        Serial.println("Erro ao iniciar SHT31!");
        while (1);
    }
    if (!bmp.begin()) {
        Serial.println("Erro ao iniciar BMP180!");
        while (1);
    }

    // Configurar Sensores de Vento e Chuva
    pinMode(WIND_SPEED_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WIND_SPEED_PIN), countWind, RISING);

    pinMode(RAIN_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RAIN_SENSOR_PIN), countRain, RISING);

    pinMode(WIND_DIR_N, INPUT_PULLUP);
    pinMode(WIND_DIR_W, INPUT_PULLUP);
    pinMode(WIND_DIR_S, INPUT_PULLUP);
    pinMode(WIND_DIR_E, INPUT_PULLUP);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Ler Temperatura e Umidade
    float temperature = sht31.readTemperature();
    float humidity = sht31.readHumidity();
    if (!isnan(temperature) && !isnan(humidity)) {
        client.publish(topic_temp, String(temperature).c_str());
        client.publish(topic_hum, String(humidity).c_str());
    }

    // Ler Pressão Atmosférica
    float pressure = bmp.readPressure() / 100.0; // Convertendo para hPa
    client.publish(topic_pressure, String(pressure).c_str());

    // Calcular Velocidade do Vento
    float wind_speed = (wind_count * 0.18); // Conversão de RPM para Km/h
    wind_count = 0;
    client.publish(topic_wind_speed, String(wind_speed).c_str());

    // Capturar Direção do Vento
    String wind_direction = getWindDirection();
    client.publish(topic_wind_direction, wind_direction.c_str());

    // Capturar Pluviosidade (Chuva)
    float rain_mm = rain_count * 0.173; // Conversão para mm de chuva
    rain_count = 0;
    client.publish(topic_rain, String(rain_mm).c_str());

    Serial.println("\U0001F4E1 Dados enviados ao MQTT!");

    delay(5000); // Publica a cada 5 segundos
}
