#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Configurações da Rede Wi-Fi
const char *ssid = "irineu";
const char *password = "12345678";

// Configuração do Servidor Backend (Substitua pelo IP do seu PC)
const char *server_url = "http://192.168.x.x:5000/api/telemetria";

// Configurações do Sensor DHT22
#define DHTPIN 4      // Pino GPIO conectado ao DATA do DHT22
#define DHTTYPE DHT22 // Tipo do sensor utilizado
DHT dht(DHTPIN, DHTTYPE);

// Configurações de Identificação do Dispositivo
const char *dispositivo_id = "ESP32_FABRICA_SETOR_A";

void setup()
{
  Serial.begin(115200);
  dht.begin();

  // Conexão com a rede Wi-Fi
  Serial.print("Conectando-se a rede Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Conectado com sucesso!");
  Serial.print("Endereço IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Aguarda 10 segundos entre os envios
  delay(10000);

  // Verifica se o Wi-Fi continua conectado antes de enviar
  if (WiFi.status() == WL_CONNECTED)
  {

    // Leitura dos dados do sensor
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    // Valida se as leituras são corretas
    if (isnan(temperatura) || isnan(umidade))
    {
      Serial.println("Falha ao ler dados do sensor DHT22!");
      return;
    }

    // Criação do objeto JSON (Capacidade estimada de 200 bytes)
    JsonDocument doc;
    doc["dispositivo_id"] = dispositivo_id;
    doc["mac_address"] = WiFi.macAddress();
    doc["temperatura_local"] = temperatura;
    doc["umidade_local"] = umidade;

    // Serializa o JSON para uma String
    String json_payload;
    serializeJson(doc, json_payload);

    // Inicializa o cliente HTTP
    HTTPClient http;
    http.begin(server_url);
    http.addHeader("Content-Type", "application/json");

    Serial.println("\n--- Enviando Requisição HTTP POST ---");
    Serial.print("Payload: ");
    Serial.println(json_payload);

    // Envia a requisição POST
    int http_response_code = http.POST(json_payload);

    // Processa o retorno do servidor
    if (http_response_code > 0)
    {
      Serial.print("Código de Status HTTP: ");
      Serial.println(http_response_code);

      String resposta_servidor = http.getString();
      Serial.print("Resposta do Servidor: ");
      Serial.println(resposta_servidor);
    }
    else
    {
      Serial.print("Erro na requisição HTTP: ");
      Serial.println(http.errorToString(http_response_code).c_str());
    }

    // Libera os recursos da requisição
    http.end();
  }
  else
  {
    Serial.println("Erro: Dispositivo desconectado do Wi-Fi.");
  }
}