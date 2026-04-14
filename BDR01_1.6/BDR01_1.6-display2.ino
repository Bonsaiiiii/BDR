#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include "base64.hpp"
#include <NTPClient.h>
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"

#define BDR_data  "/BDR.json"

#define size_data  256

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     DRST // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS DSP_ADDR ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

const uint8_t bitmap_wifi[] = {0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x06, 0x00, 0x73, 0x00, 0x79, 0x00, 0x0d, 0x80, 0x6d, 0x80, 0x6d, 0x80, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};

void fixo() {
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("PWR:");
    display.print("Modo: ");
    display.print(cModo);
    display.setCursor(74, 0);
    display.print("Saida:");
    switch(stateOUT){
      case 0: display.print("OFF");
      break;
      case 1: display.print("ON");
      break;
      case 2: display.print("N/A");
      break;
    }
    display.drawLine(0, 8, 128, 8, 1);
    display.drawLine(0, 55, 128, 55, 1);
    display.setCursor(0, 57);
    display.print("PAGE");
    display.setCursor(98, 57);
    display.print("ENTER");
    display.display();
}

void fixoConfirm() {
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("Modo: ");
    display.print(cModo);
    display.setCursor(74, 0);
    display.print("Saida:");
    switch(stateOUT){
      case 0: display.print("OFF");
      break;
      case 1: display.print("ON");
      break;
      case 2: display.print("N/A");
      break;
    }
    display.drawLine(0, 8, 128, 8, 1);
    display.drawLine(0, 55, 128, 55, 1);
    display.setCursor(0, 57);
    display.print("NO");
    display.setCursor(98, 57);
    display.print("YES");
    display.display();
}

void fixo2() {
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("Modo: ");
    display.print(cModo);
    display.setCursor(74, 0);
    display.print("Saida:");
    switch(stateOUT){
      case 0: display.print("OFF");
      break;
      case 1: display.print("ON");
      break;
      case 2: display.print("N/A");
      break;
    }
    display.drawLine(0, 8, 128, 8, 1);
    display.drawLine(0, 55, 128, 55, 1);
    display.setCursor(0, 57);
    display.print("SELECT");
    display.setCursor(98, 57);
    display.print("ENTER");
    display.display();
}

void page1(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(35, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("WIFI INFO");
    display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
    display.setCursor(12, 21);
    display.print("NOME:");
    display.print(ssid);
    display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
    display.setCursor(12, 33);
    display.print("SENHA:");
    display.print(password);
    display.drawBitmap(0, 42, bitmap_client, 11, 11, 1);
    display.setCursor(12, 44);
    display.print("Clientes:");
    display.print(WiFi.softAPgetStationNum());
    display.display();
}

void page2() {
  if (Modo == "Local") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("NTRIPHost:");
    display.print(hostNtpL);
    display.setCursor(0, 22);
    display.print("Port:");
    display.print(portNtpL);
    display.setCursor(65, 22);
    display.print("User:");
    display.print(userL);
    display.setCursor(0, 32);
    display.print("Password:");
    display.print(senL);
    display.setCursor(0, 42);
    display.print("MountPoint:");
    display.print(mtpntL);
    display.display();
  } else if (Modo == "Cliente") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("NTRIPHost:");
    display.print(hostNtpC);
    display.setCursor(0, 22);
    display.print("Port:");
    display.print(portNtpC);
    display.setCursor(65, 22);
    display.print("User:");
    display.print(userC);
    display.setCursor(0, 32);
    display.print("Password:");
    display.print(senC);
    display.setCursor(0, 42);
    display.print("MountPoint:");
    display.print(mtpntC);
    display.display();
  } else if (Modo == "Source") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("NTRIPHost:");
    display.print(hostNtpS);
    display.setCursor(0, 22);
    display.print("Port:");
    display.print(portNtpS);
    display.setCursor(65, 22);
    display.print("User:");
    display.print(userS);
    display.setCursor(0, 32);
    display.print("Password:");
    display.print(senS);
    display.setCursor(0, 42);
    display.print("MountPoint:");
    display.print(mtpntS);
    display.display();
  }
}

void page3() {
  if (Modo == "Local") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.print(baudrateL);
    display.setCursor(0, 32);
    display.print("Parity:None");
    display.setCursor(70, 32);
    display.print("Data:");
    display.print(datbitL);
    display.setCursor(0, 42);
    display.print("Stop:" + bitsParaL);
    display.setCursor(70, 42);
    display.print("Flow:None");
    display.display();
  } else if (Modo == "Cliente") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.print(baudrateC);
    display.setCursor(0, 32);
    display.print("Parity:None");
    display.setCursor(70, 32);
    display.print("Data:");
    display.print(datbitC);
    display.setCursor(0, 42);
    display.print("Stop:" + bitsParaC);
    display.setCursor(70, 42);
    display.print("Flow:None");
    display.display();
  } else if (Modo == "Source") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.print(baudrateS);
    display.setCursor(0, 32);
    display.print("Parity:None");
    display.setCursor(70, 32);
    display.print("Data:");
    display.print(datbitS);
    display.setCursor(0, 42);
    display.print("Stop:" + bitsParaS);
    display.setCursor(70, 42);
    display.print("Flow:None");
    display.display();
  }
}

void page3select(String SB) {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.setCursor(66, 22);
    display.print(SB);
    display.setCursor(0, 32);
    display.print("Parity:None");
    display.setCursor(70, 32);
    display.print("Data:");
    display.print(datbitS);
    display.setCursor(0, 42);
    display.print("Stop:" + bitsParaS);
    display.setCursor(70, 42);
    display.print("Flow:None");
    display.display();
}

void page4(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Model:BDR01 FIRM:V1.5");
    display.setCursor(0, 22);
    display.print("Local NTRIP Bridge");
    display.setCursor(0, 32);
    display.print("power output:fix(aut)");
    display.setCursor(0, 42);
    display.print("Voltage set:");
    display.setCursor(74,42);
    display.print(PDvolt);
    display.display();
}

void page4select(String PDv){
  display.fillRect(0, 10, 128, 45, 0);
  display.setCursor(0, 12);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Model:BDR01 FIRM:V1.5");
  display.setCursor(0, 22);
  display.print("Local NTRIP Bridge");
  display.setCursor(0, 32);
  display.print("power output:fix(aut)");
  display.setCursor(0, 42);
  display.print("Voltage set:");
  display.setCursor(74,42);
  display.print(PDv);
  display.display();
}

void page4confirm(String PDvC){
  display.fillRect(0, 10, 128, 45, 0);
  display.setCursor(0, 12);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Warning check the max");
  display.setCursor(0, 22);
  display.print("voltage that the GNSS");
  display.setCursor(0, 32);
  display.print("receiver can handle!");
  display.setCursor(0, 42);
  display.print("Voltage set:");
  display.setCursor(74,42);
  display.print(PDvC);
  display.display();
}

void pageLoc() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setCursor(0, 12);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Location Used:");
  if (sendLatLong == "S") {
    display.print("Yes");
  } else if (sendLatLong == "N") {
    display.print("No");
  }
  display.setCursor(0, 22);
  display.print("Lati:");
  display.print(lat.substring(0, 5));
  display.setCursor(65, 22);
  display.print("Long:");
  display.print(lon.substring(0, 5));
  display.setCursor(0, 32);
  display.print("Alti:");
  display.print(alt.substring(0, 5));
  display.setCursor(65, 32);
  display.print("Prec:");
  display.print(pre.substring(0, 5));
  display.setCursor(0, 42);
  display.print("UTC Time:");
  display.print(utc);
  display.display();
}

void page_pd(){
    display.fillRect(0, 0, 128, 64, 0);
    display.setCursor(5, 2);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Fonte de Alimentacao");
    display.setCursor(10, 12);
    display.print("USB Power Delivery");
    display.setCursor(30, 22);
    display.print("Compativel?");
    display.setCursor(20, 32);
    display.print("9V:");
    if(V9==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(70, 32);
    display.print("12V:");
    if(V12==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(17, 42);
    display.print("15V:");
    if(V15==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(67, 42);
    display.print("20V:");
    if(V20==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.display();
}
