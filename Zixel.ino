#include "DataBase.h"
#include "IR.h"
#include "Relogio.h"
#include "Display.h"
#include "Touch.h"
#include "WiFi.h"
#include "OTA.h"
#include "LittleFS.h"
#include "Info.h"
#include "Fonts.h"
#include "Menu.h"
#include "Porta.h"
#include "TCL.h"

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
  tft.init();
  tft.setRotation(0);

  conexao();
  setupIR();
  setupRelogio();
  setupTouch();
  setupTCL();

  //setupFS();
  //setupInfo();
  setupOTA();
}

void loop() {
  loopIR();
  loopRelogio();
  loopDisplay();
  loopTouch();
  loopPorta();
  loopTCL();

  //loopFS();
  //loopInfo();
  loopOTA();
}
