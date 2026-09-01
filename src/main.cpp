// 1. INCLUSÃO DE BIBLIOTECAS
#include <WiFi.h>         // Biblioteca para gerenciar o wi-fi do ESP32
#include <PubSubClient.h> // Biblioteca para o protocolo MQTT

// 2. CONFIGURAÇÃO DE REDE E SERVIDOR
const char *ssid = "SENAI-DEV2";          // Nome da rede Wi-Fi
const char *password = "desenvolvimento"; // Senha da rede Wi-Fi

// Endereço IP do computador do professor (Broker)
const char *mqtt_server = "192.168.0.103";
const int mqtt_port = 1883; // Porta padrão de MQTT

// Variáveis para controlar o tempo de envio sem usar delay()
unsigned long anteriorMillis = 0;
const long intervalo = 15000; // Tempo em milissegundos (15 segundos)

// 3. CRIAÇÃO DE OBJETOS
WiFiClient espClient;           // Objeto que gerencia a conexão TCP
PubSubClient client(espClient); // Objeto MQTT que usa o espClient para se comunicar

// --- PARTE A: A FUNÇÃO QUE DECIDE O QUE FAZER (CALLBACK) ---
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensagem chegou no tópico: ");
  Serial.println(topic);

  String mensagem;
  for (int i = 0; i < length; i++)
  {
    mensagem += (char)payload[i]; // Constrói a string a partir dos bytes
  }

  Serial.print("Conteúdo da mensagem: ");
  Serial.println(mensagem);

  // Lógica para ligar/desligar o LED no pino D4
  if (mensagem == "LIGAR")
  {
    digitalWrite(4, HIGH); // Liga o LED no pino D4
    Serial.println("LED LIGADO!");
  }
  else if (mensagem == "DESLIGAR")
  {
    digitalWrite(4, LOW); // Desliga o LED no pino D4
    Serial.println("LED DESLIGADO!");
  }
}

// --- FUNÇÃO: CONECTAR AO WI-FI ---
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

      // --- PARTE C: ASSINAR O CANAL (SUBSCRIBE) ---
      client.subscribe("sala/comando");
      Serial.println("Inscrito no tópico 'sala/comando' com sucesso!");
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
  Serial.begin(115200); // Inicia a comunicação serial

  // Configura o pino 4 (D4) do LED como saída
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW); // Garante que o LED comece apagado

  setup_wifi();                             // Chama a função de Wi-Fi
  client.setServer(mqtt_server, mqtt_port); // Configura o endereço do servidor MQTT

  // --- PARTE B: ONDE LIGAR A ESCUTA (SET CALLBACK) ---
  client.setCallback(callback);
}

// --- LOOP PRINCIPAL (RODA SEM PARAR) ---
void loop()
{
  // Se o cliente perder a conexão com o servidor, tenta reconectar
  if (!client.connected())
  {
    reconnect();
  }
  client.loop(); // Mantém a comunicação ativa e escuta os comandos em tempo real

  // --- LÓGICA DE ENVIO SEM TRAVAR O ESP32 (Utilizando Millis) ---
  unsigned long atualMillis = millis();
  if (atualMillis - anteriorMillis >= intervalo)
  {
    anteriorMillis = atualMillis;

    Serial.println("Enviando teste de conexão...");

    // Defina seu tópico e sua mensagem aqui:
    String topico = "teste/status";
    String mensagemEnvio = "Aluna Maria Fernanda conectada!";

    // Envia a mensagem para o tópico definido
    client.publish(topico.c_str(), mensagemEnvio.c_str());
    Serial.println("Mensagem enviada!");
  }
}