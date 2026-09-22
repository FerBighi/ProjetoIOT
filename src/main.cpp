// 3:
//  #include <Arduino.h>
//  #include <WiFi.h>
//  #include <HTTPClient.h>
//  #include <ArduinoJson.h>

// const char *ssid = "Irineu";
// const char *password = "12345678";

// // API ViaCEP em HTTP direto (Porta 80 - sem redirecionamento 308/301)
// const char *serverUrlGET = "http://viacep.com.br/ws/17704602/json/";

// void setup()
// {
//   Serial.begin(115200);
//   delay(1000);

//   Serial.println("\n=== ESP32 WI-FI - CONSULTA DE CEP REAL (HTTP) ===");

//   WiFi.mode(WIFI_STA);

//   Serial.print("MAC Address do ESP32: ");
//   Serial.println(WiFi.macAddress());

//   Serial.print("Conectando ao Wi-fi: ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\n[Wi-fi] Conectado com sucesso!");
//   Serial.print("[Wi-fi] IP Atribuído: ");
//   Serial.println(WiFi.localIP());
// }

// void loop()
// {
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     HTTPClient http;

//     Serial.println("\n----------------------------------");
//     Serial.print("[HTTP GET] Consultando ViaCEP: ");
//     Serial.println(serverUrlGET);

//     http.begin(serverUrlGET);
//     int httpCode = http.GET();

//     if (httpCode > 0)
//     {
//       Serial.printf("[HTTP GET] Código de Resposta do Servidor: %d\n", httpCode);

//       if (httpCode == HTTP_CODE_OK)
//       {
//         String payload = http.getString();
//         Serial.println("[HTTP GET] Resposta JSON recebida:");
//         Serial.println(payload);

//         JsonDocument doc;
//         DeserializationError error = deserializeJson(doc, payload);

//         if (!error)
//         {
//           const char *cep = doc["cep"];
//           const char *logradouro = doc["logradouro"];
//           const char *bairro = doc["bairro"];
//           const char *cidade = doc["localidade"];
//           const char *uf = doc["uf"];
//           const char *ddd = doc["ddd"];

//           Serial.println("\n-------------------------------------------");
//           Serial.println(" 📌 ENDEREÇO LOCALIZADO REAL (ViaCEP)");
//           Serial.println("-------------------------------------------");
//           Serial.printf(" CEP        : %s\n", cep);
//           Serial.printf(" Logradouro : %s\n", logradouro);
//           Serial.printf(" Bairro     : %s\n", bairro);
//           Serial.printf(" Cidade/UF  : %s/%s\n", cidade, uf);
//           Serial.printf(" DDD        : %s\n", ddd);
//           Serial.println("-------------------------------------------");
//         }
//         else
//         {
//           Serial.print("[JSON Error] Falha no parsing: ");
//           Serial.println(error.c_str());
//         }
//       }
//     }
//     else
//     {
//       Serial.printf("[HTTP GET] Falha. Erro: %s\n", http.errorToString(httpCode).c_str());
//     }

//     http.end();
//   }

//   delay(15000);
// }

// 4:
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Irineu";
const char *password = "12345678";

// URL HTTPS da AwesomeAPI
const char *serverUrlHTTPS = "https://awesomeapi.com.br";

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESP32 WI-FI - COTAÇÃO DO DÓLAR REAL (HTTPS) ===");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[Wi-Fi] Conectado!");
  Serial.print("[Wi-Fi] IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    // 1. Criar o cliente seguro SSL/TLS
    WiFiClientSecure client;
    client.setInsecure(); // Ignora a verificação estrita de certificado SSL no ESP32

    HTTPClient http;

    Serial.println("\n--------------------------------------------------");
    Serial.print("[HTTPS GET] Consultando AwesomeAPI: ");
    Serial.println(serverUrlHTTPS);

    // 2. Inicializar a conexão passando o cliente seguro
    http.begin(client, serverUrlHTTPS);

    int httpCode = http.GET();

    if (httpCode > 0)
    {
      Serial.printf("[HTTPS GET] Código de Resposta do Servidor: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        Serial.println("[HTTPS GET] Resposta JSON recebida:");
        Serial.println(payload);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error)
        {
          JsonObject usd = doc["USDBRL"];

          const char *nomeMoeda = usd["name"];
          const char *valorCompra = usd["bid"];
          const char *valorVenda = usd["ask"];
          const char *maximaDia = usd["high"];
          const char *minimaDia = usd["low"];
          const char *variacao = usd["pctChange"];
          const char *dataHora = usd["create_date"];

          Serial.println("\n--------------------------------------------------");
          Serial.printf(" 💵 COTAÇÃO EM TEMPO REAL: %s\n", nomeMoeda);
          Serial.println("--------------------------------------------------");
          Serial.printf(" Valor de Compra (Bid) : R$ %s\n", valorCompra);
          Serial.printf(" Valor de Venda (Ask)  : R$ %s\n", valorVenda);
          Serial.printf(" Máxima do Dia (High)  : R$ %s\n", maximaDia);
          Serial.printf(" Mínima do Dia (Low)   : R$ %s\n", minimaDia);
          Serial.printf(" Variação do Dia       : %s%%\n", variacao);
          Serial.printf(" Última Atualização    : %s\n", dataHora);
          Serial.println("--------------------------------------------------");
        }
        else
        {
          Serial.print("[JSON Error] Falha no parsing: ");
          Serial.println(error.c_str());
        }
      }
    }
    else
    {
      Serial.printf("[HTTPS GET] Falha na requisição. Erro: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(15000);
}

// // DESAFIO 1 DO MÓDULO
// // #include <Arduino.h>

// // // Definição dos pinos
// // const int pinoBotao = 13;
// // const int pinoRele = 26;

// // // Variáveis de estado
// // bool estadoRele = false;
// // int estadoBotaoAtual;
// // // HIGH porque usamos o resistor interno pull-up
// // int estadoBotaoAnterior = HIGH;

// // unsigned long tempoFisico = 0;

// // void setup()
// // {
// //   Serial.begin(115200);

// //   pinMode(pinoBotao, INPUT_PULLUP);
// //   pinMode(pinoRele, OUTPUT);

// //   // Garante que o rele inicie desligado
// //   digitalWrite(pinoRele, LOW);

// //   Serial.print("Sistema iniciado, Rele DESLIGADO");
// // }

// // void loop()
// // {
// //   // Lê o estado do botão
// //   int leitura = digitalRead(pinoBotao);

// //   // Se o botão realmente mudou em relação á última leitura confirmada
// //   if (leitura != estadoBotaoAtual)
// //   {
// //     estadoBotaoAtual = leitura;

// //     // Só altera o rele quando o botão é pressionado (vai para o LOW)
// //     if (estadoBotaoAtual == LOW)
// //     {
// //       estadoRele = !estadoRele; // Inverte o estado do rele
// //       digitalWrite(pinoRele, estadoRele ? HIGH : LOW);

// //       // Exibir o status no Monitor Serial
// //       if (estadoRele)
// //       {
// //         Serial.println("Rele Ligado");
// //         tempoFisico = millis();
// //       }
// //       else
// //       {
// //         Serial.println("Rele Desligado");
// //       }
// //     }
// //   }

// //   if (estadoRele && (millis() - tempoFisico >= 10000))
// //   {
// //     estadoRele = false;
// //     digitalWrite(pinoRele, LOW);
// //     Serial.println("Rele Desligado Automaticamente");
// //   }
// // }

// // DESAFIO CARGA 1
// // #include <Arduino.h>

// // // Definição dos pinos (Alinhado com o ESP32 do projeto)
// // const int pinoBotao = 13;
// // const int pinoRele = 26;

// // // Configuração do Hardware do Relé (Suporte Active-LOW - Desafio Anterior)
// // const int RELE_LIGADO = LOW;     // Módulos Active-LOW ligam em 0V
// // const int RELE_DESLIGADO = HIGH; // Módulos Active-LOW desligam em VCC

// // // Variáveis de estado e controle
// // bool estadoRele = false;
// // int estadoBotaoAnterior = HIGH; // Resistor interno PULL-UP inicia em HIGH

// // // Variáveis para Debounce por Software (Essencial para Validação de Bancada)
// // unsigned long ultimoTempoDebounce = 0;
// // const unsigned long tempoDebounce = 50; // 50 milissegundos para estabilizar o botão

// // void setup()
// // {
// //   // Inicialização do Monitor Serial na velocidade especificada
// //   Serial.begin(1152000);

// //   // Configuração dos pinos conforme diretrizes de segurança MSEP
// //   pinMode(pinoBotao, INPUT_PULLUP);
// //   pinMode(pinoRele, OUTPUT);

// //   // Garante isolamento inicial: Relé em repouso (Desligado)
// //   digitalWrite(pinoRele, RELE_DESLIGADO);

// //   Serial.println("==================================================");
// //   Serial.println("MSEP - Sistema de Automacao de Cargas com ESP32");
// //   Serial.println("Status: Inicializado. Rele em REPOUSO.");
// //   Serial.println("Sinalizacao: LED Vermelho (NC) ACESO | LED Verde (NO) APAGADO");
// //   Serial.println("==================================================");
// // }

// // void loop()
// // {
// //   // Leitura imediata do pino do botão
// //   int leituraAtual = digitalRead(pinoBotao);

// //   // Verifica se houve mudança de estado físico no botão
// //   if (leituraAtual != estadoBotaoAnterior)
// //   {
// //     ultimoTempoDebounce = millis(); // Reinicia o cronômetro do debounce
// //   }

// //   // Se o estado se manteve estável por mais tempo que o 'tempoDebounce'
// //   if ((millis() - ultimoTempoDebounce) > tempoDebounce)
// //   {

// //     // Verifica se a transição foi de solto para PRESSIONADO (Borda de descida -> LOW)
// //     if (leituraAtual == LOW && estadoRele == false)
// //     {

// //       // Altera o estado lógico interno
// //       estadoRele = true;

// //       // Comuta fisicamente o Relé para a posição de Operação
// //       digitalWrite(pinoRele, RELE_LIGADO);

// //       Serial.println("[ALERTA] Botao Pressionado. Rele ATIVADO!");
// //       Serial.println("Sinalizacao: LED Verde (NO) ACESO | LED Vermelho (NC) APAGADO");

// //       // --- TEMPORIZADOR DE SEGURANÇA INDUSTRIAL (MANTIDO COM DELAY) ---
// //       delay(10000); // Retém a carga ativa por exatamente 10 segundos

// //       // Força o desligamento automático após o tempo limite
// //       estadoRele = false;
// //       digitalWrite(pinoRele, RELE_DESLIGADO);

// //       Serial.println("[SEGURANCA] Tempo limite atingido. Rele DESLIGADO automaticamente.");
// //       Serial.println("Sinalizacao: LED Vermelho (NC) RETORNOU AO STANDBY.");
// //       Serial.println("==================================================");
// //     }
// //   }

// //   // Atualiza o histórico do botão para o próximo ciclo do loop
// //   estadoBotaoAnterior = leituraAtual;
// // }

// // DESAFIO CARGA 2
// #include <Arduino.h>

// // ============================================================================
// // CONFIGURAÇÃO DE HARDWARE (MSEP - DIRETRIZES DE BANCADA)
// // ============================================================================
// const int pinoBotao = 13; // Entrada do botão de comando (com Resistor Pull-up)
// const int pinoRele = 26;  // Saída de controle do Relé (Isolamento Galvânico)

// // Configuração para módulos relé industriais comuns (Active-LOW)
// // (Caso use um relé Active-HIGH, basta inverter HIGH e LOW nestas duas constantes)
// const int RELE_LIGADO = LOW;
// const int RELE_DESLIGADO = HIGH;

// // ============================================================================
// // VARIÁVEIS DE ESTADO E DEBOUNCE (FILTRAGEM DE BANCADA)
// // ============================================================================
// bool estadoRele = false;        // Controla o estado lógico da bobina do relé
// int estadoBotaoAnterior = HIGH; // Histórico do botão para detectar cliques
// int ultimoEstadoEstavel = HIGH; // Estado filtrado pós-debounce

// unsigned long ultimoTempoDebounce = 0;  // Armazena o tempo do último ruído mecânico
// const unsigned long tempoDebounce = 50; // Tempo de filtro (50ms)

// void setup()
// {
//   // Inicialização do Monitor Serial para diagnósticos da planta
//   Serial.begin(1152000);

//   // Configuração elétrica dos pinos
//   pinMode(pinoBotao, INPUT_PULLUP);
//   pinMode(pinoRele, OUTPUT);

//   // ESTADO INICIAL SEGURO: Garante que o relé inicie DESLIGADO (Em repouso)
//   digitalWrite(pinoRele, RELE_DESLIGADO);

//   Serial.println("==================================================");
//   Serial.println("     SENAI-SP - INTERNET DAS COISAS (75h)         ");
//   Serial.println("   DESAFIO 2: SISTEMA INDUSTRIAL DE SINALIZACAO   ");
//   Serial.println("==================================================");
//   Serial.println("[ESTADO INICIAL] Exaustor Industrial: DESLIGADO");
//   Serial.println("[SINALIZACAO HW] LED Vermelho (NC) ACESO [Standby]");
//   Serial.println("[SINALIZACAO HW] LED Verde (NO) APAGADO");
//   Serial.println("==================================================");
// }

// void loop()
// {
//   // 1. LEITURA DO BOTÃO DE COMANDO
//   int leituraAtual = digitalRead(pinoBotao);

//   // 2. FILTRO DE DEBOUNCE (Evita acionamentos falsos gerados pelo botão)
//   if (leituraAtual != estadoBotaoAnterior)
//   {
//     ultimoTempoDebounce = millis(); // Reinicia temporizador se houver oscilação
//   }

//   // Verifica se o sinal do botão estabilizou
//   if ((millis() - ultimoTempoDebounce) > tempoDebounce)
//   {

//     // Detecta o momento exato em que o botão foi pressionado (borda de descida)
//     if (leituraAtual == LOW && ultimoEstadoEstavel == HIGH)
//     {

//       // Inverte o estado lógico da bobina do relé
//       estadoRele = !estadoRele;

//       // Comuta fisicamente o Relé
//       digitalWrite(pinoRele, estadoRele ? RELE_LIGADO : RELE_DESLIGADO);

//       // 3. FEEDBACK NO MONITOR SERIAL (Reflete fielmente a comutação física)
//       if (estadoRele)
//       {
//         Serial.println("\n[COMANDO] Operador ligou a maquina.");
//         Serial.println("[STATUS]  Exaustor Industrial: EM OPERACAO");
//         Serial.println("[FÍSICA]   Contato COM -> NO: LED Verde [ACESO] | LED Vermelho [APAGADO]");
//       }
//       else
//       {
//         Serial.println("\n[COMANDO] Operador desligou a maquina.");
//         Serial.println("[STATUS]  Exaustor Industrial: EM STANDBY");
//         Serial.println("[FÍSICA]   Contato COM -> NC: LED Vermelho [ACESO] | LED Verde [APAGADO]");
//       }
//       Serial.println("--------------------------------------------------");
//     }

//     // Atualiza o estado estável validado
//     ultimoEstadoEstavel = leituraAtual;
//   }

//   // Atualiza o histórico para o próximo ciclo do loop
//   estadoBotaoAnterior = leituraAtual;
// }