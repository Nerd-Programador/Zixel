#ifndef WIFI_H
#define WIFI_H

#include "DataBase.h"
#include "OTA.h"
#include "LittleFS.h"

// Variáveis para controlar o tempo de espera para a conexão Wi-Fi
unsigned long tempoInicio = 0;
const unsigned long tempoMaximo = 10000;  // Tempo máximo de espera em milissegundos (10 segundos)

// Configuração da conexão WiFi
const char* ssid = "Marina";                  // Insira o nome da sua rede Wi-Fi
const char* pass = "1Betania2Doidinhos";  // Insira a senha da sua rede Wi-Fi
const IPAddress ip(192, 168, 15, 99);         // Endereço IP fixo
const IPAddress gateway(192, 168, 15, 1);     // Endereço IP do gateway
const IPAddress subnet(255, 255, 255, 0);     // Máscara de sub-rede
const IPAddress dns(8, 8, 8, 8);

void conexao() {
  // Conexão à rede Wi-Fi
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet, dns);
  tempoInicio = millis();  // Armazena o tempo inicial

  while (WiFi.status() != WL_CONNECTED) {
    // Verifica se o tempo de espera excedeu o tempo máximo
    if (millis() - tempoInicio > tempoMaximo) {
      Serial.println("Tempo de conexão expirado. Falha ao conectar-se à rede Wi-Fi.");
      break;  // Sai do loop
    }

    // Aguarda um curto período antes de verificar novamente o status da conexão
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }

  // Verifica se a conexão foi estabelecida com sucesso
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conexão bem-sucedida à rede Wi-Fi!");
  } else {
    // Trate a falha na conexão aqui, se necessário
    Serial.println("Falha na conexão à rede Wi-Fi.");
    ESP.reset();  // Reinicia o ESP8266 em caso de falha na conexão
  }
}

#endif