#ifndef OTA_H
#define OTA_H

#include "DataBase.h"
#include "WiFi.h"
#include "LittleFS.h"

const char* hostname = "Zixel";

void setupOTA() {

  ArduinoOTA.setPassword("258096");

  ArduinoOTA.begin();

  // Inicializar o mDNS
  if (!MDNS.begin(hostname)) {
    Serial.println("Erro ao inicializar mDNS");
    while (1) {
      delay(1000);
    }
  } else {
    Serial.println("mDNS iniciado");
    Serial.println(hostname);
  }

  // Definir a função de callback para reiniciar o ESP8266 após a atualização
  ArduinoOTA.onEnd([]() {
    Serial.println("Atualização OTA concluída. Reiniciando ESP8266...");
    ESP.restart();
  });
}

void loopOTA() {

  ArduinoOTA.handle();
}

#endif