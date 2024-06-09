#ifndef TOUCH_H
#define TOUCH_H

#include "DataBase.h"
#include "IR.h"

// Inicialização do botão touch e do LED
void setupTouch() {
  pinMode(PIN_TOUCH, INPUT_PULLUP);
  touch.attach(PIN_TOUCH); // Anexa o pino do botão ao objeto Bounce
  touch.interval(25); // Define o intervalo de debounce em milissegundos
}

// Função para acender todos os LEDs
void turnOnAllLeds() {
  for (int i = 0; i < COUNT_LED; i++) {
    lumen.setBrightness(BRILHO_DEFAULT);
    lumen.setPixelColor(i, lumen.Color(255, 255, 255)); // Cor branca
  }
  lumen.show(); // Mostra as cores
}

// Função para apagar todos os LEDs
void turnOffAllLeds() {
  lumen.clear(); // Apaga todas as cores
  lumen.show(); // Mostra as cores apagadas
}

// Função para mudar o estado de todos os LEDs
void toggleLumen() {
  lumenState = !lumenState; // Alterna entre ligado e desligado
  if (lumenState) {
    // Se o LED estiver ligado, acenda todos os LEDs
    turnOnAllLeds();
  } else {
    // Se o LED estiver desligado, apague todos os LEDs
    turnOffAllLeds();
  }
}

// Função para verificar o toque no botão e controlar o LED
void loopTouch() {
  // Atualiza o estado do objeto Bounce
  touch.update();
  
  // Verifica se houve uma mudança de estado no botão
  if (touch.fell()) {
    // Se o botão foi pressionado
    toggleLumen(); // Muda o estado do LED
  }
}

#endif