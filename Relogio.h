#ifndef RELOGIO_H
#define RELOGIO_H

#include "DataBase.h"
#include "Fonts.h"

// NTP Servers:
static const char ntpServerName[] = "time.windows.com";
const int timeZone = -3;  // Fuso

extern TFT_eSPI tft;

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
time_t prevDisplay = 0;
void sendNTPpacket(IPAddress &address);

void setupRelogio() {
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  tft.fillScreen(TFT_BLACK);
}

// Definir a posição do texto do relógio
int posX = 0;
int posY = 0;

void digitalDate() {
  // Exibir a data e o dia da semana com cores personalizadas
  String DayOfWeek;
  switch (weekday()) {
    case 1: DayOfWeek = "Domingo"; break;
    case 2: DayOfWeek = "Segunda"; break;
    case 3: DayOfWeek = "Terca"; break;
    case 4: DayOfWeek = "Quarta"; break;
    case 5: DayOfWeek = "Quinta"; break;
    case 6: DayOfWeek = "Sexta"; break;
    case 7: DayOfWeek = "Sabado"; break;
  }

  // Exibir a data
  String currentDay = (day() < 10 ? "0" : "") + String(day());
  String currentMonth = String(month() < 10 ? "0" : "") + String(month());
  String currentDate = DayOfWeek + ", " + currentDay + "/" + currentMonth;  // + "/" + String(year());
  static String lastDate = "";                                              // Salvar a última data exibida
  if (currentDate != lastDate) {                                            // Verificar se a data mudou
    lastDate = currentDate;
    tft.setTextColor(TFT_WHITE);
    tft.fillRect(posX, posY, 240, 27, TFT_BLACK);  // Limpar a área da data e exibir a nova data
    tft.setCursor(posX, posY + 21);
    tft.setFreeFont(&DEJAVU_8);  // 28 DejaVu_Serif_Condensed_Bold_28
    tft.print(currentDate);
  }
}

void digitalClock() {
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&DSEG7_9);  // 70 DSEG7_Classic_Mini_Bold_70

  // Exibir a hora
  String currentHour = (hour() < 10 ? "0" : "") + String(hour());
  //String currentSecond = (second() < 10 ? "0" : "") + String(second());
  static String lastHour = "";    // Salvar a última hora exibida
  if (currentHour != lastHour) {  // Verificar se a hora mudou
    lastHour = currentHour;
    tft.fillRect(posX, posY + 30, 105, 70, TFT_BLACK);  // Limpar a área da hora e exibir a nova hora
    tft.setCursor(posX - 3, posY + 100);
    tft.print(currentHour);
  }

  // Exibir os minutos
  String currentMinute = (minute() < 10 ? "0" : "") + String(minute());
  static String lastMinute = "";      // Salvar o último minuto exibido
  if (currentMinute != lastMinute) {  // Verificar se o minuto mudou
    lastMinute = currentMinute;
    tft.fillRect(posX + 137, posY + 30, 103, 70, TFT_BLACK);  // Limpar a área da hora e exibir a nova hora
    tft.setCursor(posX + 130, posY + 100);
    tft.print(currentMinute);
  }

  // Atualiza apenas a região dos :
  static bool showColon = true;  // Flag para controlar o piscar dos :
  String currentTime = (showColon ? ":" : " ");
  tft.setCursor(posX + 114, posY + 100);  // Ajuste a posição conforme necessário
  tft.fillRect(posX + 107, posY + 30, 28, 70, TFT_BLACK);
  tft.print(showColon ? ":" : " ");  // Desenha ":" ou espaço

  showColon = !showColon;  // Alterna o estado do :
}

void loopRelogio() {
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) {  // Atualiza a exibição somente se a hora tiver mudado
      prevDisplay = now();
      digitalClock();
      digitalDate();
    }
  }
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;      // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming & outgoing packets

time_t getNtpTime() {
  IPAddress ntpServerIP;  // NTP server's ip address

  while (Udp.parsePacket() > 0)
    ;
  WiFi.hostByName(ntpServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 = (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;  // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;  // LI, Version, Mode
  packetBuffer[1] = 0;           // Stratum, or type of clock
  packetBuffer[2] = 6;           // Polling Interval
  packetBuffer[3] = 0xEC;        // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123);  //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

#endif