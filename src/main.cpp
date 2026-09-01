// 1. INCLUSÃO DE BIBLIOTECAS
#include <WIFI.h>         // Biblioteca para gerenciar o wi-fi do ESP32
#include <PubSubClient.h> // Biblioteca para o protocolo MQTT

// 2. CONFIGURAÇÃO DE REDE E SERVIDOR
const char *ssid = "SENAI-DEV2";          // Nome da rede Wi-Fi
const char *password = "desenvolvimento"; // Senha da rede Wi-Fi

// Endereço IP do computador do professor (Broker) - será passado na hora
const char *mqtt_server = "192.168.0.100";
const int mqtt_port = 1883; // Porta padrão de MQTT

// 3. CRIAÇÃO DE OBJETOS
WiFiClient espClient;           // Objeto que gerencia a conexão TCP
PubSubClient client(espClient); // Objeto MQTT que usa o espClient para se comunicar

// ---FUNÇÃO: CONECTAR AO WI-FI ---
void setup_wifi()
{
  delay(10);
  Serial.println("\nConectando ao Wi-Fi...");

  WiFi.begin(ssid, password); // Inicia a conexão com a rede Wi-Fi

  // Enquanto não conectar, imprime um ponto no monitor serial
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Conectado com sucesso!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP()); // Mostra o IP que o ESP32 recebeu
}

// --- FUNÇÃO: RECONECTAR AO BROKER MQTT ---
void reconnect()
{
  // Loop que roda até que a conexão com o servidor seja estabelecida
  while (!client.connected())
  {
    Serial.print("Tentando conectar ao Broker MQTT...");

    // Gerar um ID único para evitar conflitos no servidor
    String clientId = "ESP32-Aluno-" + String(random(0xffff), HEX);

    // Tenta conectar
    if (client.connect(clientId.c_str()))
    {
      Serial.println("CONECTADO AO SERVIDOR!");
    }
    else
    {
      Serial.print("Falha ao conectar. Erro: ");
      Serial.print(client.state()); // Mostra o código de erro
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// --- CONFIGURAÇÃO INICIAL (RODA UMA VEZ) ---
void setup()
{
  Serial.begin(115200);                     // Inicia a comunicação serial para vermos as mensagens
  setup_wifi();                             // Chama a função de Wi-Fi
  client.setServer(mqtt_server, mqtt_port); // Configura o endereço do servidor MQTT
}

// --- LOOP PRINCIPAL (RODA SEM PARAR) ---
void loop()
{
  // Se o cliente perder a conexão com o servidor, tenta reconectar
  if (!client.connected())
  {
    reconnect();
  }
  client.loop(); // Mantém a comunicação ativa com o servidor

  // --- LÓGICA DE ENVIO DE MENSAGEM ---
  Serial.println("Enviando teste de conexão...");

  // Defina seu tópico e sua mensagem aqui:
  String topico = "teste/status";
  String mensagem = "Aluna Maria Fernanda conectado!";

  // Envia a mensagem para o tópico definido
  client.publish(topico.c_str(), mensagem.c_str());

  Serial.println("Mensagem enviada! Aguardando 15 segundos...");
  delay(15000); // Espera 15 segundos para não sobrecarregar o servidor
}