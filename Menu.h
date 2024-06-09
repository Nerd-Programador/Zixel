#ifndef MENU_H
#define MENU_H

#include "DataBase.h"
#include "IR.h"
#include "Display.h"
#include "Relogio.h"
#include "Fonts.h"
#include "Info.h"

#include <stack>
#include <vector>

extern TFT_eSPI tft;
extern decode_results results;
extern unsigned long lastIRTime;
extern unsigned long debounceDelay;

int posH = 0;
int posV = 125;

int currentMenuIndex = 0;
int firstVisibleIndex = 0;  // Índice da primeira opção visível
bool inMenu = false;

// Funções para desenhar e manipular o menu
void drawMenu();
void navigateMenu(int direction);
void selectMenuOption();
void exitMenu();

#define clearMenu posH, posV - 23, tft.width(), tft.height() - posV - 8, TFT_BLACK

#define VISIBLE_MENU_OPTIONS 5  // Número de opções visíveis de uma vez

enum MenuType {
  MAIN_MENU,
  LED_MENU,
  PHOTO_MENU,
  SYSTEM_MENU
};

MenuType currentMenu = MAIN_MENU;
std::stack<MenuType> menuStack;

// Definições das opções dos menus
std::vector<String> mainMenuOptions = {
  "Fotos", "LEDs", "Sistema", "Configuracoes", "Sobre"
};
std::vector<String> ledMenuOptions = {
  "Pulso de Cor", "Piscar Aleatório", "Arco-íris", "Respiração", "Fade In/Out"
};
std::vector<String> photoMenuOptions = {
  "Slide", "Foto 1", "Foto 2", "Foto 3", "Foto 4",
  "Foto 5", "Foto 6", "Foto 7", "Foto 8",
  "Foto 9", "Foto 10", "Foto 11", "Foto 12"
};

// Função para Desenhar Menus
void drawMenu() {
  tft.fillRect(clearMenu);
  // Desenhar o título do menu
  tft.setFreeFont(&DEJAVU_8); // DejaVu_Serif_Condensed_Bold_28
  tft.setTextColor(TFT_GOLD);
  tft.setCursor(posH + 80, posV);
  tft.fillRect(posH, posV - 21, +240, 22, TFT_BLUE);

  std::vector<String>* menuOptions;
  switch (currentMenu) {
    case MAIN_MENU:
      tft.print("Menu");
      menuOptions = &mainMenuOptions;
      break;
    case LED_MENU:
      tft.print("LEDs");
      menuOptions = &ledMenuOptions;
      break;
    case PHOTO_MENU:
      tft.print("Fotos");
      menuOptions = &photoMenuOptions;
      break;
    case SYSTEM_MENU:
      tft.print("Sistema");
      break;
  }

  tft.setFreeFont(&DEJAVU_5); //DejaVu_Serif_Condensed_Bold_18

  // Desenhar as opções do menu
  for (int i = 0; i < VISIBLE_MENU_OPTIONS; i++) {
    int optionIndex = firstVisibleIndex + i;
    if (optionIndex >= menuOptions->size()) break;  // Não desenhe fora do limite

    if (optionIndex == currentMenuIndex) {
      tft.setTextColor(TFT_RED);
    } else {
      tft.setTextColor(TFT_SILVER);
    }
    tft.setCursor(posH, posV + 25 + i * 25);
    tft.print(menuOptions->at(optionIndex));
  }
}

// Função para Navegar nos Menus
void navigateMenu(int direction) {
  currentMenuIndex += direction;

  int menuOptionCount;
  switch (currentMenu) {
    case MAIN_MENU:
      menuOptionCount = mainMenuOptions.size();
      break;
    case LED_MENU:
      menuOptionCount = ledMenuOptions.size();
      break;
    case PHOTO_MENU:
      menuOptionCount = photoMenuOptions.size();
      break; /*
    case 
      menuOptionCount = .size();
      break;
    case 
      menuOptionCount = .size();
      break;
    case 
      menuOptionCount = .size();
      break;*/
  }

  if (currentMenuIndex < 0) {
    currentMenuIndex = menuOptionCount - 1;
    firstVisibleIndex = max(0, menuOptionCount - VISIBLE_MENU_OPTIONS);
  } else if (currentMenuIndex >= menuOptionCount) {
    currentMenuIndex = 0;
    firstVisibleIndex = 0;
  } else if (currentMenuIndex < firstVisibleIndex) {
    firstVisibleIndex = currentMenuIndex;
  } else if (currentMenuIndex >= firstVisibleIndex + VISIBLE_MENU_OPTIONS) {
    firstVisibleIndex = currentMenuIndex - VISIBLE_MENU_OPTIONS + 1;
  }
  drawMenu();
}

// Função para Selecionar Opções
void selectMenuOption() {
  switch (currentMenu) {
    case MAIN_MENU:
      switch (currentMenuIndex) {
        case 0:
          menuStack.push(MAIN_MENU);
          currentMenu = PHOTO_MENU;
          break;
        case 1:
          menuStack.push(MAIN_MENU);
          currentMenu = LED_MENU;
          break;
        case 2:
          menuStack.push(MAIN_MENU);
          tft.fillRect(clearMenu);
          system();
          return;
          // Adicione mais casos conforme necessário
      }
      break;
      // Função para Desenhar os Sub-Menus
    case LED_MENU:
      if (currentMenuIndex == 5) {
        currentMenu = menuStack.top();
        menuStack.pop();
      } else {
      }
      break;
    case PHOTO_MENU:
      if (currentMenuIndex == 5) {
        currentMenu = menuStack.top();
        menuStack.pop();
      } else {
        //tft.fillRect(clearMenu);
      }
      break;
  }
  drawMenu();
}

void exitMenu() {
  inMenu = false;
  // Limpa a área onde o menu estava desenhado ao sair do menu
  tft.fillRect(clearMenu);
}

void loopMenu() {
  unsigned long timeLeitura = millis();
  if (timeLeitura - lastIRTime >= debounceDelay) {
    if (irrecv.decode(&results)) {
      unsigned long irCode = results.value;

      if (irCode == BTN_C) {
        navigateMenu(-1);  // Navegar para cima
      } else if (irCode == BTN_B) {
        navigateMenu(+1);  // Navegar para baixo
      } else if (irCode == CONFIR) {
        selectMenuOption();  // Confirmar opção
      } else if (irCode == VOLTAR) {
        if (!menuStack.empty()) {
          currentMenu = menuStack.top();
          menuStack.pop();
          currentMenuIndex = 0;
          firstVisibleIndex = 0;
          drawMenu();
        } else {
          exitMenu();  // Voltar para a tela principal
        }
      }
    }
    irrecv.resume();  // Recebe o próximo valor
  }
}

#endif