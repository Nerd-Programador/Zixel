#ifndef PORTA_H
#define PORTA_H

#include "DataBase.h"
#include "WiFi.h"
#include "Display.h"
#include "IR.h"
#include "Fonts.h"

extern TFT_eSPI tft;

#define clearMSG 0, 289, 240, 31, TFT_BLACK

// IP da Porta
const char* host = "http://192.168.15.50:50";

// Variáveis para controle de tempo
unsigned long previousMillisSensores = 0;
unsigned long intervalSensores = 30000;  // Intervalo para leitura dos sensores (5 segundos)

// Variáveis para armazenar os dados dos sensores
float temperatura = 0;
float umidade = 0;
float altitude = 0;

// Variáveis para armazenar os valores anteriores dos sensores
float oldTemperatura = 0;
float oldUmidade = 0;
float oldAltitude = 0;

// Variáveis para exibição de mensagens temporárias
unsigned long previousMillisMensagem = 0;  // Armazena o tempo em que a mensagem começou a ser exibida
unsigned long intervalMensagem = 0;        // Define o período pelo qual a mensagem deve ser exibida
bool mensagemAtiva = false;                // Flag para indicar se uma mensagem está sendo exibida
String mensagem = "";                      // Armazena a mensagem a ser exibida

// Declaração das funções
void exibirMensagemTemporaria(String msg, unsigned long tempo);
void displaySensor();
void drawCircularProgressBar(int x, int y, int radius, float value, float minValue, float maxValue, uint16_t color);
void sensores();
void abrir();
void fechar();

void sensores() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, String(host) + "/data");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Dados dos sensores: " + payload);

      // Parse dos dados JSON (assumindo que o payload é JSON)
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        temperatura = doc["temperature"];
        umidade = doc["humidity"];
        altitude = doc["externalTemperature"];
      } else {
        Serial.println("Erro ao fazer o parse dos dados JSON");
      }
    } else {
      Serial.println("Erro ao buscar dados dos sensores");
    }
    http.end();
  }
}

void abrir() {
  exibirMensagemTemporaria("Abrindo...", 3000);  // Exibe a mensagem imediatamente

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, String(host) + "/comando?comando=A");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Resposta Abrir: " + payload);
    } else {
      Serial.println("Erro ao enviar comando Abrir");
    }
    http.end();
  }
}

void fechar() {
  exibirMensagemTemporaria("Fechando...", 5000);  // Exibe a mensagem imediatamente

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, String(host) + "/comando?comando=F");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Resposta Fechar: " + payload);
    } else {
      Serial.println("Erro ao enviar comando Fechar");
    }
    http.end();
  }
}

void exibirMensagemTemporaria(String msg, unsigned long tempo) {
  mensagem = msg;                     // Armazena a mensagem recebida
  intervalMensagem = tempo;           // Define o período de exibição
  previousMillisMensagem = millis();  // Armazena o tempo atual
  mensagemAtiva = true;               // Define a flag como verdadeira

  tft.fillRect(clearMSG);  // Limpa a área da mensagem no display
  tft.setFreeFont(&CALLI_5); // Calligraffitti_Regular_30
  tft.setCursor(65, 315);
  tft.setTextColor(TFT_RED);
  tft.print(mensagem);  // Exibe a mensagem no display
}

void displaySensor() {
  // Atualiza a tela apenas se os valores mudaram
  if (oldTemperatura != temperatura || oldUmidade != umidade || oldAltitude != altitude) {
    tft.fillRect(0, 115, 240, 171, TFT_BLACK);

    // Exibe os gráficos circulares de progresso em forma de triângulo
    drawCircularProgressBar(65, 160, 45, temperatura, -10, 50, TFT_ORANGE);
    drawCircularProgressBar(177, 160, 45, umidade, 0, 100, TFT_BLUE);
    drawCircularProgressBar(123, 240, 45, altitude, -10, 50, TFT_GREEN);

    tft.setFreeFont(&CALLI_8); // Calligraffitti_Regular_30

    // Exibe os dados dos sensores dentro dos círculos
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(40, 177);
    tft.print((int)temperatura);  // Remove casas decimais

    tft.setTextColor(TFT_CYAN);
    tft.setCursor(149, 177);
    tft.print((int)umidade);  // Remove casas decimais

    tft.setTextColor(TFT_GREEN);
    tft.setCursor(95, 257);
    tft.print((int)altitude);  // Remove casas decimais

    // Atualiza os valores antigos
    oldTemperatura = temperatura;
    oldUmidade = umidade;
    oldAltitude = altitude;
  }
}

void drawCircularProgressBar(int x, int y, int radius, float value, float minValue, float maxValue, uint16_t color) {
  // Fundo do gráfico
  tft.drawCircle(x, y, radius, TFT_LIGHTGREY);
  tft.drawCircle(x, y, radius - 1, TFT_LIGHTGREY);
  tft.drawCircle(x, y, radius - 2, TFT_LIGHTGREY);
  tft.drawCircle(x, y, radius - 3, TFT_LIGHTGREY);

  // Barra de progresso
  float percent = (value - minValue) / (maxValue - minValue);
  percent = percent < 0 ? 0 : (percent > 1 ? 1 : percent);  // Garante que percent esteja no intervalo [0, 1]
  int endAngle = percent * 360;

  for (int i = 0; i <= endAngle; i++) {
    // Ajuste do ângulo para iniciar na posição 6 (parte inferior) e ir no sentido horário
    float angle = (i + 90) * DEG_TO_RAD;  // Desloca a posição inicial para 6 e inverte o sentido
    for (int j = 0; j < 5; j++) {         // Desenha os 4 pixels de espessura
      int x0 = x + (radius - j) * cos(angle);
      int y0 = y + (radius - j) * sin(angle);
      tft.drawPixel(x0, y0, color);
    }
  }
}

void loopPorta() {
  unsigned long currentMillis = millis();

  // Chama a função de leitura de sensores a cada intervalo definido
  if (currentMillis - previousMillisSensores >= intervalSensores) {
    previousMillisSensores = currentMillis;
    sensores();
    displaySensor();
  }

  // Exibe mensagem temporária se necessário
  if (mensagemAtiva && currentMillis - previousMillisMensagem >= intervalMensagem) {
    mensagemAtiva = false;   // Redefine a flag para indicar que a mensagem não está mais ativa
    tft.fillRect(clearMSG);  // Limpa a área da mensagem no display
    displaySensor();         // Redesenha os dados dos sensores
  }
}

#endif