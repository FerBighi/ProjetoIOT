#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Configurações do Wi-Fi
const char *ssid = "Irineu";
const char *password = "12345678";

// URL da API REST de Clima (Exemplo: Presidente Prudente)
const char *serverUrlGET = "http://wttr.in";

// Configurações do Sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== DESAFIO 1: ESTAÇÃO METEOROLÓGICA HÍBRIDA ===");

  // Inicializa o sensor DHT22
  dht.begin();

  // Conexão Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[Wi-Fi] Conectado!");
  Serial.print("[Wi-Fi] Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // 1. Leitura Local (Sensor DHT22)
  float tempLocal = dht.readTemperature();
  float umidLocal = dht.readHumidity();

  // Variáveis para armazenar os dados da API externa
  String tempExterna = "N/A";
  String sensacaoTermica = "N/A";
  String umidExterna = "N/A";
  String ventoVelocidade = "N/A";
  String condicaoTempo = "N/A";

  // 2. Requisição HTTP GET (Clima Externo via API REST)
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(serverUrlGET);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();

      // Aloca memória dinamicamente para o JSON grande do wttr.in
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error)
      {
        // Navegação na estrutura JSON específica do wttr.in
        JsonObject current_condition = doc["current_condition"][0];

        tempExterna = current_condition["temp_C"].as<String>();
        sensacaoTermica = current_condition["FeelsLikeC"].as<String>();
        umidExterna = current_condition["humidity"].as<String>();
        ventoVelocidade = current_condition["windspeedKmph"].as<String>();

        // A condição traduzida fica dentro de lang_pt
        condicaoTempo = current_condition["lang_pt"][0]["value"].as<String>();
      }
      else
      {
        Serial.print("[JSON] Falha ao desserializar: ");
        Serial.println(error.c_str());
      }
    }
    else
    {
      Serial.printf("[HTTP GET] Falha na requisição. Erro: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else
  {
    Serial.println("[Wi-Fi Error] Conexão Wi-Fi perdida!");
  }