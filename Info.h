#ifndef INFO_H
#define INFO_H

#include "DataBase.h"
#include "Fonts.h"
#include "WiFi.h"
#include "LittleFS.h"
#include "OTA.h"

extern int posH;
extern int posV;

// Variável global para armazenar a versão do OS
const String OS_VERSION = "1.7";

// Obtém a versão do firmware
String firmwareVersion = ESP.getCoreVersion();

// Obtém as informações do hardware
String chipId = String(ESP.getChipId());
String coreVersion = String(ESP.getCoreVersion());
String flashChipRealSize = String(ESP.getFlashChipRealSize());
String freeSketchSpace = String(ESP.getFreeSketchSpace());
String cpuFreqMHz = String(ESP.getCpuFreqMHz());
String flashChipId = String(ESP.getFlashChipId());

void system() {
  tft.setFreeFont(&DEJAVU_1); // DejaVu_Serif_Condensed_Bold_10
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(posH, posV);

  tft.println("Versão: " + String(OS_VERSION));
  tft.println();

  // Imprime informações do hardware no display TFT
  tft.println("Informações do Hardware");
  tft.println("ID do Chip: " + chipId);
  tft.println("Versão do Firmware: " + coreVersion);
  tft.println("Endereço MAC: " + WiFi.macAddress());
  tft.println();

  tft.println("Informações da Memória Flash:");
  tft.println("  - Capacidade Total: " + flashChipRealSize + " bytes");
  tft.println("  - Espaço Livre: " + freeSketchSpace + " bytes");

  // Cálculo do espaço ocupado na memória flash
  uint32_t flashChipSize = ESP.getFlashChipSize();
  uint32_t flashChipUsed = flashChipSize - ESP.getFreeSketchSpace();
  tft.println("  - Espaço Ocupado: " + String(flashChipUsed) + " bytes");
  tft.println();

  tft.println("Informações da CPU:");
  tft.println("  - Modelo: " + cpuFreqMHz + " MHz");
  tft.println();

  tft.println("Informações da GPU:");
  tft.println("  - Modelo: " + flashChipId);
  tft.println("Frequência de Clock: " + cpuFreqMHz + " MHz");
  tft.println("RAM Disponível: " + String(ESP.getFreeHeap()) + " bytes");
  tft.println();

  // Imprime informações da rede Wi-Fi
  tft.println("Informações da Rede:");
  tft.println("Rede Wi-Fi Conectada: " + WiFi.SSID());
  tft.println("IPv4: " + WiFi.localIP().toString());
  tft.println("Força do Sinal Wi-Fi:");
  // tft.println();

}

unsigned long tempoInicial;  // Variável para armazenar o tempo inicial

void activity() {
  unsigned long tempoAtual = millis();                       // Obtém o tempo atual
  unsigned long tempoDecorrido = tempoAtual - tempoInicial;  // Calcula o tempo decorrido

  // Converte o tempo decorrido para horas e minutos
  unsigned int horas = tempoDecorrido / 3600000;
  unsigned int minutos = (tempoDecorrido % 3600000) / 60000;
  
  // Exibe no tft TFT SPI
  tft.setFreeFont(&DEJAVU_1); // DejaVu_Serif_Condensed_Bold_10
  tft.setCursor(posH, posV); // Posição do cursor
  tft.setTextColor(TFT_BLUE); // Cor do texto
  tft.print("Tempo ligado: "); // Texto inicial
  if (horas < 10) tft.print("0");
  tft.print(horas); // Horas
  tft.print(":");
  if (minutos < 10) tft.print("0"); // Adiciona um zero à esquerda se necessário
  tft.print(minutos); // Minutos
}

void setupInfo() {

  tempoInicial = millis();  // Armazena o tempo inicial

}

void loopInfo() {
  // Obtém a força do sinal Wi-Fi
  int rssi = WiFi.RSSI();

  // Atualiza a linha no display TFT com a nova força do sinal
  tft.setCursor(0, 202);                                           // Define a posição no display para exibir a força do sinal
  tft.fillRect(0, 202, tft.width(), tft.fontHeight(), TFT_BLACK);  // Limpa a área do texto anterior
  tft.print(rssi);                                                 // Exibe a nova força do sinal
  tft.print(" dBm");
}

#endif