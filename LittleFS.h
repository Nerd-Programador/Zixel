#ifndef LITTLEFS_H
#define LITTLEFS_H
/*
#include "DataBase.h"
#include "OTA.h"
#include "WiFi.h"

ESP8266WebServer server(80);

extern TFT_eSPI tft;
void logMemoryStatus();

void drawBMP(const char *filename, int16_t x, int16_t y);
uint16_t read16(File &f);
uint32_t read32(File &f);

void setupFS() {
  if (!LittleFS.begin()) {
    Serial.println("Falha ao montar o sistema de arquivos LittleFS");
    return;
  }
  // Configuração do servidor web
  server.on("/log", HTTP_GET, []() {
    File logFile = LittleFS.open("/log.txt", "r");
    if (logFile) {
      server.streamFile(logFile, "text/plain");
      logFile.close();
    } else {
      server.send(404, "text/plain", "Arquivo de log não encontrado");
    }
  });

  server.begin();
  Serial.println("Servidor web iniciado!");

  //logMemoryStatus();
}

// Função para adicionar uma entrada ao arquivo de log
void adicionarLog(String mensagem) {
  // Abre o arquivo de log em modo de escrita (append)
  File file = LittleFS.open("/log.txt", "a");

  // Verifica se o arquivo foi aberto com sucesso
  if (!file) {
    Serial.println("Erro ao abrir o arquivo de log!");
    return;
  }

  // Escreve a mensagem no arquivo
  file.println(mensagem);

  // Fecha o arquivo
  file.close();
  // Verifica o tamanho do arquivo de log e apaga se for muito grande
  file = LittleFS.open("/log.txt", "r");
  if (file.size() > 20000) {  // Exemplo: 20 KB
    file.close();
    LittleFS.remove("/log.txt");
  } else {
    file.close();
  }
}

// Função para ler o conteúdo completo do arquivo de log
void lerLog() {
  // Abre o arquivo de log em modo de leitura
  File file = LittleFS.open("/log.txt", "r");

  // Verifica se o arquivo foi aberto com sucesso
  if (!file) {
    Serial.println("Erro ao abrir o arquivo de log!");
    return;
  }

  // Lê e imprime cada linha do arquivo
  while (file.available()) {
    Serial.write(file.read());
  }

  // Fecha o arquivo
  file.close();
}

// Função para remover o arquivo de log (se necessário)
void limparLog() {
  // Remove o arquivo de log
  if (LittleFS.remove("/log.txt")) {
    Serial.println("Arquivo de log removido com sucesso!");
  } else {
    Serial.println("Erro ao remover o arquivo de log!");
  }
}

void drawBMP(const char *filename, int16_t x, int16_t y) {
  if ((x >= tft.width()) || (y >= tft.height())) return;

  File bmpFile;
  int bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageoffset;
  uint32_t rowSize;
  bool goodBmp = false;

  Serial.print("Abrindo arquivo BMP: ");
  Serial.println(filename);

  bmpFile = LittleFS.open(filename, "r");

  if (!bmpFile) {
    Serial.print("Arquivo não encontrado: ");
    Serial.println(filename);
    return;
  }

  if (read16(bmpFile) == 0x4D42) { // BMP signature
    read32(bmpFile);
    read32(bmpFile);
    bmpImageoffset = read32(bmpFile);
    read32(bmpFile);
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    if (read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);
      if ((bmpDepth == 16) && (read32(bmpFile) == 0)) {
        goodBmp = true;
        rowSize = (bmpWidth * 2 + 3) & ~3; // Para 16 bits, são 2 bytes por pixel
        bmpHeight = abs(bmpHeight);

        tft.startWrite();
        tft.setAddrWindow(x, y, bmpWidth, bmpHeight);

        uint8_t* lineBuffer = (uint8_t*)malloc(bmpWidth * 2);
        if (lineBuffer) {
          for (int row = 0; row < bmpHeight; row++) {
            uint32_t pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            bmpFile.seek(pos);
            bmpFile.read(lineBuffer, bmpWidth * 2);

            uint16_t* bptr = (uint16_t*)lineBuffer;
            for (int col = 0; col < bmpWidth; col++) {
              uint16_t color = *bptr++;
              tft.pushColor(color);
            }
          }
          free(lineBuffer);
        }
        tft.endWrite();
        Serial.println("Imagem desenhada com sucesso.");
      } else {
        Serial.println("Formato BMP não suportado. Apenas BMPs de 16 bits são suportados.");
      }
    } else {
      Serial.println("Número de planos não suportado.");
    }
  } else {
    Serial.println("Assinatura BMP não encontrada.");
  }

  bmpFile.close();
  if (!goodBmp) Serial.println("Formato BMP não reconhecido.");
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();  // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();  // MSB
  return result;
}

void loopFS() {
  server.handleClient();
  logMemoryStatus();
}
*/
#endif