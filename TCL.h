#ifndef TCL_H
#define TCL_H

#include "DataBase.h"
#include "Display.h"
#include "IR.h"

extern TFT_eSPI tft;
extern void updateDisplay(); // Declaração externa

IRsend emissorIR(pinEmissor);  // Objeto para envio de IR

bool timerStarted = false;
unsigned long startTime = 0;
unsigned long timerDuration = 10 * 60 * 1000;  // 10 minutos em milissegundos
unsigned long currentEndTime = 0;             // Tempo final atual
unsigned long lastDisplayedMinutes = 0;  // Armazena o último valor de minutos exibido

uint16_t rawTCL[] = { 4000, 4000, 550, 1950, 550, 2000, 550, 1950, 550, 2000, 550, 1000, 500, 1000, 550, 2000, 500, 1050, 500, 2000, 550, 1000, 500, 2000, 550, 1000, 550, 1000, 500, 1100, 500, 1000, 550, 1000, 550, 1950, 550, 2000, 550, 1000, 500, 2000, 550, 1000, 550, 2000, 500, 1000, 550, 2000, 500 };

void setupTCL() {
  emissorIR.begin();  // Inicializa o envio de IR
  //status.begin();  // Inicializa o LED de status
  //status.show();   // Define o LED como apagado inicialmente
}

void SinalIR() {
  emissorIR.sendRaw(rawTCL, sizeof(rawTCL) / sizeof(rawTCL[0]), 38);  // 38 é a frequência em kHz
}
/*
void setStatusColor(uint8_t red, uint8_t green, uint8_t blue) {
  status.setPixelColor(0, status.Color(red, green, blue));
  status.show();
}
*/
void checkAdd() {
  //Verifica se o botão de adicionar tempo foi pressionado.
  //Se pressionado, inicia o temporizador ou adiciona 10 minutos ao tempo atual.
  static unsigned long lastButtonPressTimeAdd = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastButtonPressTimeAdd > 250) {  // Debounce de 250 ms
    lastButtonPressTimeAdd = currentTime;

    if (!timerStarted) {
      timerStarted = true;
      startTime = currentTime;
      currentEndTime = startTime + timerDuration;
      Serial.println("Temporizador iniciado");
      //setStatusColor(COR_PURPLE);  // Define o LED para PURPLE
    } else {
      currentEndTime += 10 * 60 * 1000;  // Adiciona 10 minutos ao tempo final
      Serial.println("Tempo estendido em mais 10 minutos");
    }
    updateDisplay(); // Chama a função updateDisplay()
  }
}

void checkSubtract() {
  //Verifica se o botão de subtrair tempo foi pressionado.
  //Se pressionado e o temporizador estiver em andamento, subtrai 2 minutos ou desliga o temporizador.
  //Se pressionado e o temporizador não estiver em andamento, envia o sinal IR imediatamente.
  static unsigned long lastButtonPressTimeSubtract = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastButtonPressTimeSubtract > 250) {  // Debounce de 250 ms
    lastButtonPressTimeSubtract = currentTime;

    if (timerStarted) {
      if ((currentEndTime - currentTime) > 10 * 60 * 1000) {
        currentEndTime -= 10 * 60 * 1000;  // Subtrai 10 minutos do tempo final
        Serial.println("Tempo reduzido em 10 minutos");
      } else {
        timerStarted = false;
        Serial.println("Temporizador desligado");
        //setStatusColor(0, 0, 0);  // Apaga o LED
      }
      updateDisplay(); // Chama a função updateDisplay()
    } else {
      // Envia a sequência de pulsos IR usando o pino D2
      SinalIR();
      Serial.println("Sinal IR enviado!");
    }
  }
}

void checkTimer() {
  //Verifica se o temporizador terminou.
  //Se o temporizador terminou, envia o sinal IR e reseta o temporizador.
  unsigned long currentTime = millis();

  if (timerStarted && currentTime >= currentEndTime) {
    // Envia a sequência de pulsos IR usando o pino D2
    SinalIR();
    Serial.println("Sinal IR enviado!");

    // Reseta o temporizador
    timerStarted = false;
    //setStatusColor(0, 0, 0);  // Apaga o LED
    updateDisplay(); // Chama a função updateDisplay()
  }
}

void loopTCL() {
  checkTimer();
}

#endif
