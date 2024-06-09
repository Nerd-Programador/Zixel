#ifndef DISPLAY_H
#define DISPLAY_H

#include "DataBase.h"
#include "IR.h"
#include "Fonts.h"
#include "Porta.h"
#include "TCL.h"

int pos_X = 20;
int pos_Y = 120;

extern TFT_eSPI tft;

/********************************************************************************************/

extern bool timerStarted;                   // Declaração externa
extern unsigned long currentEndTime;        // Declaração externa
extern unsigned long lastDisplayedMinutes;  // Armazena o último valor de minutos exibido

unsigned long lastUpdateTime = 0;  // Armazena o tempo da última atualização do display
bool displayActive = false;  // Indica se o display está ativo

void clearDisplay() {
  tft.fillRect(pos_X - 20, pos_Y + 170, 240, 32, TFT_BLACK);
  displayActive = false;
}

void updateDisplay() {
  unsigned long currentTime = millis();
  unsigned long remainingTime = (currentEndTime - currentTime) / 1000;  // Tempo restante em segundos

  if (timerStarted && remainingTime > 0) {
    int minutes = remainingTime / 60;
    tft.fillRect(pos_X - 20, pos_Y + 170, 240, 32, TFT_BLACK);  // Limpa a área antes de desenhar
    tft.setFreeFont(&CALLI_5);
    tft.setCursor(pos_X - 20, pos_Y + 197);
    tft.printf("Temporizador: %d min", minutes);

    lastUpdateTime = currentTime;
    displayActive = true;
  } else {
    clearDisplay();
  }
}

/********************************************************************************************/

// Defina as coordenadas para a barra de progresso
#define BAR_START 20
#define BAR_END 220

// Defina os valores mínimo e máximo de brilho
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 250

static unsigned long lastInteractionTime = 0;  // Armazena o tempo da última interação
static bool brightnessBarActive = false;       // Flag para rastrear se a barra de brilho está ativa

void drawBrightnessBar(int brightness) {
  brightnessBarActive = true;  // Marca a barra de brilho como ativa
  // Calcule a posição do indicador na barra de progresso
  int barWidth = BAR_END - BAR_START;
  int midPoint = (BAR_START + BAR_END) / 2;
  int indicatorX = map(brightness, 10, 250, BAR_START, BAR_END);

  // Desenha a barra de progresso vazia
  tft.drawRect(BAR_START, pos_Y + 180, barWidth, pos_X, TFT_GREEN);

  // Limpa a área preenchida anterior
  tft.fillRect(BAR_START + 1, pos_Y + 182, barWidth - 2, pos_X - 4, TFT_BLACK);

  // Desenha o preenchimento do meio para a direita
  int fillRightWidth = map(brightness, 125, 250, 0, (BAR_END - midPoint) - 6);
  if (fillRightWidth > 0) {
    tft.fillRect(midPoint + 1, pos_Y + 182, fillRightWidth, pos_X - 4, TFT_CYAN);
  }

  // Desenha o preenchimento do meio para a esquerda
  int fillLeftWidth = map(brightness, 10, 125, (midPoint - BAR_START) - 2, 0);
  if (fillLeftWidth > 0) {
    tft.fillRect(midPoint - fillLeftWidth - 1, pos_Y + 182, fillLeftWidth, pos_X - 4, TFT_CYAN);
  }

  // Desenha os valores acima da barra de progresso
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Configura a cor do texto (branco com fundo preto)
  tft.setTextSize(0);                      // Configura o tamanho do texto
  tft.setFreeFont(NULL);

  // Desenha "10" na posição inicial da barra
  tft.drawString("10", BAR_START - 5, pos_Y + 170);

  // Desenha "125" no meio da barra
  tft.drawString("125", (BAR_START + BAR_END) / 2 - 10, pos_Y + 170);

  // Desenha "250" na posição final da barra
  tft.drawString("250", BAR_END - 10, pos_Y + 170);
}

// Função para limpar a área
void clearArea() {
  tft.fillRect(pos_X - 20, pos_Y + 170, 240, 31, TFT_BLACK);
  brightnessBarActive = false;  // Marca a barra de brilho como inativa após limpar a área
}

void loopDisplay() {
  // Limpar a área da barra de brilho após 5 segundos de inatividade
  if (brightnessBarActive && millis() - lastInteractionTime >= 5000) {
    clearArea();
  }

  // Limpa a área do display após 5 segundos de inatividade
  if (displayActive && millis() - lastUpdateTime >= 5000) {
    clearDisplay();
  }
}

/********************************************************************************************/
#endif