#ifndef DATABASE_H
#define DATABASE_H

/************************************************************************************************/
// Bibliotecas utilizadas
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <Bounce2.h>
#include <FS.h>
#include <LittleFS.h>
// #include <ESP8266WebServer.h>
// #include <SD.h>

#include "Fonts.h"
#include "LittleFS.h"

/************************************************************************************************/
// Definições dos pinos GPIO utilizados
#define PIN_TOUCH D0    // 16
#define PIN_IR D1       // 5
#define PIN_LED 3      // RX
#define LED_RESPOSTA 1  // TX
#define pinEmissor D2  // 4
//#define PIN_STATUS 9  // S3

#define COUNT_LED 11
#define COUNT_STATUS 1

/************************************************************************************************/
// Definições dos Objetos globais
Adafruit_NeoPixel lumen(COUNT_LED, PIN_LED, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel status(COUNT_STATUS, PIN_STATUS, NEO_GRB + NEO_KHZ800);
IRrecv irrecv(PIN_IR);
TFT_eSPI tft = TFT_eSPI();
Bounce touch = Bounce();

/************************************************************************************************/
// Definições de variáveis globais
extern int BRILHO_DEFAULT;     // Brilho padrão
extern int BRILHO_UP;          // Incrementar brilho
extern int BRILHO_DOWN;        // Decrementar brilho
extern int currentBrightness;  // Brilho atual

bool lumenState = false;

uint32_t color1 = 0xFF0000;  // Vermelho
uint32_t color2 = 0x00FF00;  // Verde
int count = 10;
int wait = 100;
uint32_t color = 0xFF00FF;  // Magenta
int duration = 1000;

/************************************************************************************************/
// Definições dos códigos IR
#define IR_ON 0x6D8867D7
#define IR_OFF 0x692D04A0
#define BTN_0 0xF32F72D7
#define BTN_1 0xC9767F76
#define BTN_2 0xC8155AB1
#define BTN_3 0xB6996DAE
#define BTN_4 0x969AE844
#define BTN_5 0x4AAFAC67
#define BTN_6 0x9C2A936C
#define BTN_7 0x833ED333
#define BTN_8 0x55F2B93
#define BTN_9 0xDE78B0D0
#define PONTOS 0x9D65B0BC
#define GUIA 0x427EBE9F
#define MENU 0x76CF1379
#define LISTA 0xAB4D94A9
#define REC 0xC332FABB
#define SAIR 0x34498102
#define CONFIR 0x75A956A7
#define INFO 0xF640360
#define VOLTAR 0xBB9BDEE7
#define MUTE 0x218A91B4
#define CH_V 0x5BFBFDE9
#define CH_MA 0x165412B7
#define CH_ME 0x5815B090
#define DASH 0xBDE97C12
#define ENTER 0x3F23F43
#define BTN_C 0xF24119FE
#define BTN_B 0xB489062B
#define BTN_E 0xC53794B4
#define BTN_D 0xBC9DF06
// TV CCE 13782
#define VOL_MA 0x5BB59ED4
#define VOL_MA2 0x970EB328
#define VOL_ME 0x940EAE71
#define VOL_ME2 0x58B59A1D
#define TV_VI 0xD0264A08
#define TV_VI2 0x94CD35B4
#define MUTE_2 0x560DBB35
#define MUTE_3 0x1AB4A6E1

//#define TCL 0x6A68351E
// Reset 981

//0x600BEA85
/************************************************************************************************/
// Definições das cores em hexadecimal
#define COR_RED 255, 0, 0
#define COR_GREEN 0, 255, 0
#define COR_BLUE 0, 0, 255
#define COR_WHITE 255, 255, 255
#define COR_YELLOW 255, 255, 0
#define COR_CYAN 0, 255, 255
#define COR_MAGENTA 55, 0, 255
#define COR_ORANGE 255, 140, 0
#define COR_PURPLE 128, 0, 128
#define COR_PINK 255, 20, 147
#define COR_TURQUOISE 175, 238, 238
#define COR_SLATE 123, 104, 238
#define COR_GOLD 255, 215, 0
#define COR_SILVER 192, 192, 192
#define COR_BROWN 165, 42, 42
#define COR_MAROON 128, 0, 0
#define COR_NAVY 0, 0, 128

/************************************************************************************************/
// Definições do controle de brilho
#define BRILHO_DEFAULT 125  // Brilho padrão
#define BRILHO_UP 10        // Incrementar brilho
#define BRILHO_DOWN 10      // Decrementar brilho

/************************************************************************************************/
// Definições de possíveis erros com base nas cores
#define ERRO_DISPLAY COR_GREEN     // Erro no display (cor verde)
#define ERRO_REDE COR_RED          // Erro de conexão de rede (cor vermelha)
#define ERRO_SENSOR COR_ORANGE     // Erro no sensor (cor laranja)
#define ERRO_HARDWARE COR_MAGENTA  // Erro de hardware (cor magenta)
#define ERRO_MEMORIA COR_PURPLE    // Erro de memória (cor roxa)
#define ERRO_GENERICO COR_WHITE    // Erro genérico (cor branco)
#define ERRO_IR COR_STATE          // Erro no sensor infravermelho (cor azul claro)
#define ERRO_TOUCH COR_TURQUOISE   // Erro no botão touch (cor turquesa)

/************************************************************************************************/
// Definições dos Efeitos de Iluminação
void setColor(int r, int g, int b) {
  for (int i = 0; i < COUNT_LED; i++) {
    lumen.setPixelColor(i, r, g, b);
  }
  lumen.show();
  //status.show();
}

//  Pulso de Cor: Esta animação faz com que os LEDs pisquem rapidamente entre duas cores.
void colorPulse(uint32_t color1, uint32_t color2, int duration) {
  unsigned long startTime = millis();
  int halfDuration = duration / 2;

  while (millis() - startTime < duration) {
    float progress = (millis() - startTime) / (float)halfDuration;
    int brightness = constrain((int)(255 * abs(2 * progress - 1)), 0, 255);

    // Interpolação linear entre as duas cores
    uint8_t r = ((color1 >> 16) & 0xFF) * (1 - progress) + ((color2 >> 16) & 0xFF) * progress;
    uint8_t g = ((color1 >> 8) & 0xFF) * (1 - progress) + ((color2 >> 8) & 0xFF) * progress;
    uint8_t b = (color1 & 0xFF) * (1 - progress) + (color2 & 0xFF) * progress;

    // Define a cor dos LEDs
    for (int i = 0; i < COUNT_LED; i++) {
      lumen.setPixelColor(i, r, g, b);
    }
    lumen.show();

    delay(10);  // Pequena pausa entre os loops para suavizar a transição
  }
}

// Piscar Aleatório: Os LEDs piscam aleatoriamente entre várias cores.
void randomBlink(int count, int duration) {
  for (int i = 0; i < count; i++) {
    int randomIndex = random(COUNT_LED);      // Escolhe um LED aleatório
    uint32_t randomColor = random(0xFFFFFF);  // Gera uma cor aleatória

    lumen.setPixelColor(randomIndex, randomColor);
    lumen.show();
    delay(duration);

    lumen.setPixelColor(randomIndex, 0);  // Apaga o LED
    lumen.show();
    delay(duration);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return lumen.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return lumen.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return lumen.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// Arco-íris: Os LEDs exibem um efeito de arco-íris, mudando suavemente entre as cores do espectro.
void rainbow(int wait) {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < COUNT_LED; i++) {
      lumen.setPixelColor(i, Wheel((i + j) & 255));
    }
    lumen.show();
    delay(wait);
  }
}

// Respiração: Os LEDs aumentam e diminuem gradualmente de intensidade, criando um efeito de "respiração".
void breathe(uint32_t color, int duration) {
  unsigned long startTime = millis();
  int halfDuration = duration / 2;

  while (millis() - startTime < duration) {
    float progress = (millis() - startTime) / (float)halfDuration;
    int brightness = constrain((int)(255 * abs(2 * progress - 1)), 0, 255);

    lumen.fill(lumen.Color(brightness, brightness, brightness));
    lumen.show();

    delay(10);  // Pequena pausa entre os loops para suavizar a transição
  }
}

// Fade In/Out: Os LEDs aumentam e diminuem gradualmente de brilho em uma cor específica.
void fadeInOut(uint32_t color, int duration) {
  unsigned long startTime = millis();
  int halfDuration = duration / 2;

  while (millis() - startTime < duration) {
    float progress = (millis() - startTime) / (float)halfDuration;
    int brightness = constrain((int)(255 * abs(2 * progress - 1)), 0, 255);

    lumen.fill(lumen.Color(brightness, brightness, brightness));
    lumen.show();

    delay(10);  // Pequena pausa entre os loops para suavizar a transição
  }
}

/************************************************************************************************/


#endif
