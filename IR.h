#ifndef IR_H
#define IR_H

#include "DataBase.h"
#include "Display.h"
#include "Touch.h"
#include "WiFi.h"
#include "OTA.h"
#include "Info.h"
#include "Porta.h"
#include "Menu.h"
#include "LittleFS.h"
#include "TCL.h"

decode_results results;
extern bool inMenu;

unsigned long lastIRTime = 0;       // Armazena o tempo da última leitura do controle remoto
unsigned long debounceDelay = 200;  // Tempo de debounce (200 milissegundos)

int currentBrightness = BRILHO_DEFAULT;

void setupIR() {
  irrecv.enableIRIn();
  lumen.begin();
  pinMode(LED_RESPOSTA, OUTPUT);  // Configura o pino do LED de resposta como saída
}

void loopIR() {
  unsigned long currentTime = millis();

  // Verifica se tempo suficiente passou desde a última leitura do controle remoto
  if (currentTime - lastIRTime >= debounceDelay) {
    if (irrecv.decode(&results)) {
      unsigned long irCode = results.value;

      // Pisca o LED de resposta
      digitalWrite(LED_RESPOSTA, HIGH);  // Liga o LED
      delay(50);
      digitalWrite(LED_RESPOSTA, LOW);  // Desliga o LED

      // Verificar o código IR recebido e executar a ação correspondente
      if (irCode == IR_ON) {
        lumenState = true;  // Definir estado do LED como ligado
        lumen.setBrightness(BRILHO_DEFAULT);
        turnOnAllLeds();  // Acender todos os LEDs
      } 
      else if (irCode == IR_OFF) {
        lumenState = false;  // Definir estado do LED como desligado
        turnOffAllLeds();    // Apagar todos os LEDs
        lumen.setBrightness(BRILHO_DEFAULT);
      } 
      else if (irCode == CH_MA) {
        currentBrightness += BRILHO_UP;  // Código IR para aumentar o brilho
        if (currentBrightness > 255) {
          currentBrightness = 255;  // Limite máximo do brilho
        }
        lumen.setBrightness(currentBrightness);
        lumen.show();
        drawBrightnessBar(currentBrightness);
        lastInteractionTime = millis();  // Atualiza o tempo da última interação
      } 
      else if (irCode == CH_ME) {
        currentBrightness -= BRILHO_DOWN;  // Código IR para diminuir o brilho
        if (currentBrightness < 10) {
          currentBrightness = 10;  // Limite mínimo do brilho
        }
        lumen.setBrightness(currentBrightness);
        lumen.show();
        drawBrightnessBar(currentBrightness);
        lastInteractionTime = millis();  // Atualiza o tempo da última interação
      } 
      else if (irCode == BTN_0) {
        setColor(COR_RED);  // Vermelho
      } 
      else if (irCode == BTN_1) {
        setColor(COR_GREEN);  // Verde
      } 
      else if (irCode == BTN_2) {
        setColor(COR_BLUE);  // Azul
      } 
      else if (irCode == BTN_3) {
        setColor(COR_YELLOW);  // Amarelo
      } 
      else if (irCode == BTN_4) {
        setColor(COR_CYAN);  // Ciano
      } 
      else if (irCode == BTN_5) {
        setColor(COR_MAGENTA);  // Magenta
      } 
      else if (irCode == BTN_6) {
        setColor(COR_ORANGE);  // Laranja
      } 
      else if (irCode == BTN_7) {
        setColor(COR_GOLD);  // Purpura
      } 
      else if (irCode == BTN_8) {
        setColor(COR_BROWN);  // Turquesa
      } 
      else if (irCode == BTN_9) {
        setColor(COR_NAVY);  // Roxo
      } 
      else if (irCode == SAIR) {
        ESP.restart();
      } 
      else if (irCode == MENU) {
        inMenu = true;
        drawMenu();
      } 
      else if (irCode == ENTER) {
        abrir();
      } 
      else if (irCode == DASH) {
        fechar();
      }
      else if (irCode == REC) {
        checkAdd();
      }
      else if (irCode == GUIA) {
        checkSubtract();
      }
      else if (irCode == PONTOS) {
        if (startTime > 0) {
          updateDisplay();
        }
      }



      // Adicione mais condições conforme necessário para os outros códigos IR definidos no DataBase.h
      if (inMenu) {
        loopMenu();
      }
      lastIRTime = currentTime;  // Atualiza o tempo da última leitura do controle remoto
      irrecv.resume();           // Reinicia o receptor de IR para aguardar o próximo sinal
    }
  }
}

#endif