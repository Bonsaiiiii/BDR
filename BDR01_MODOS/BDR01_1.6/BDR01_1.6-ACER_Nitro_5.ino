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

#define CF3 4
#define CF2 5
#define CF1 6
#define PDPG 7
#define LED2B 15
#define LED2R 16
#define LED2G 17
#define ENTER 3
#define PAGE 13
#define POUT 9
#define SDA 10
#define SCL 11
#define DRST 12
#define DPWR 14
#define DET 21
#define TX 2
#define RX 1
#define LED1B 43
#define LED1R 44
#define LED1G 42
#define DSP_ADDR 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     DRST // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS DSP_ADDR ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define EXTINV  37
#define EXTOFF  36
#define EXTEN   35

// imagens
const uint8_t bitmap_wifi[] = {0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x06, 0x00, 0x73, 0x00, 0x79, 0x00, 0x0d, 0x80, 0x6d, 0x80, 0x6d, 0x80, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};




/// importante
const char* res_rev1 = "ICY 200 OK";
const char* res_rev2 = "HTTP/1.1 200 OK\r\n";
const char* res_rev1_ST = "SOURCETABLE 200 OK";

// modo local
const char* mountpoint = "BDR0";
const char* usuario = "HGNP";
const char* senha  = "0102";
uint16_t port = 2101;


// podem repetir
String ssid;
String password;
String SBAUD;
bool FLAG_Stable;
bool FLAG_MP;
bool flag_send;
int readcount;
bool flag_ok = false;
unsigned long currentMillis;
unsigned long atualMillis;
unsigned long wait;

char ch[512];
char cliente_data[512];

int devices=0;
int devicesOld=0;
int a=0;
int b=0;
int c=0;
bool attfixo;
bool attpage;
bool attpage3;
bool attpage4;
bool flag_page3;
bool flag_page4;
bool flag_select_serial;
bool flag_select_power;
bool pausa;
bool enter;
bool V5;
bool V9;
bool V12;
bool V15;
bool V20;
bool confirm;
bool flag_confirm;
bool cansel;
int stateOUT= 0; // 0=off, 1=ON, 2=N/A
int statePD= 0; // 0=5V, 1=9V, 2=12V, 3=15V, 4=20V
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 
unsigned long button_time2 = 0;  
unsigned long last_button_time2 = 0; 
unsigned long button_time3 = 0; 
unsigned long last_button_time3 = 0; 
String BAUDS[5]={"9600","19200","38400","57600","115200"};
String MODES[3]={"Local","Cliente","Source"};
String volt[5]={"5V","9V","12V","15V","20V"};
String PDvolt = "12V";
String OldPDvolt;
String MODE = "Local";
char cJSBAUD[7]; char cJVSET [4]; char cJMODES[8];


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiServer wifiServer(port);
WiFiClient client = wifiServer.available();

bool ext_com(bool state){
  switch(state){
    case 1:
      digitalWrite(EXTEN, LOW);
      digitalWrite(EXTOFF, HIGH);
      break;
    case 0:
      digitalWrite(EXTEN, HIGH);
      digitalWrite(EXTOFF, LOW);
      break;
  }
  return digitalRead(EXTINV);
}

bool loaddata(String filename){
  if (SPIFFS.begin(false) || SPIFFS.begin(true)){
    if (SPIFFS.exists(filename)){
      if(filename==BDR_data){
        File configFile = SPIFFS.open(filename, "r");
          if (configFile){
            StaticJsonDocument<size_data> json;
            DeserializationError error = deserializeJson(json, configFile);
            if (!error){
              //canais de transmissão
              SBAUD = strcpy(cJSBAUD, json["JSBAUD"]);
              PDvolt = strcpy(cJVSET, json["JVSET"]);
              MODE = strcpy(cJMODES, json["JMODES"]);
              return true;
          }else{
              Serial.println("Failed to load BDR_data");
          }
        }else{
          Serial.println("Failed to load BDR_data");
        }
      }else{
        Serial.println("filename dont match");
      }
      
    }
  }else{
    Serial.println("Failed to mount FS");
  }
  return false;
}

void saveConfigFile(String filename){
  if(filename==BDR_data){
    StaticJsonDocument<size_data> json;
    json["JSBAUD"] = SBAUD.c_str();
    json["JVSET"] = PDvolt.c_str();
    json["JMODES"] = MODE.c_str();
    File configFile = SPIFFS.open(filename, "w");
    if (!configFile){
      Serial.println("failed to open BDR_data file for writing");
    }
    if (serializeJson(json, configFile) == 0){
      Serial.println(F("Failed to write to file"));
    }
    configFile.close();
  }
}

//pins initialization
void pins_init(){
  pinMode(CF3, OUTPUT);
  pinMode(CF2, OUTPUT);
  pinMode(CF1, OUTPUT);
  pinMode(PDPG, INPUT);
  pinMode(LED2B, OUTPUT);
  pinMode(LED2R, OUTPUT);
  pinMode(LED2G, OUTPUT);
  pinMode(LED1B, OUTPUT);
  pinMode(LED1R, OUTPUT);
  pinMode(LED1G, OUTPUT);
  pinMode(PAGE, OUTPUT);
  pinMode(ENTER, OUTPUT);
  pinMode(POUT, OUTPUT);
  pinMode(DRST, OUTPUT);
  pinMode(DPWR, OUTPUT);
  pinMode(DET, INPUT);
}

void usbpd(int volt){
  switch(volt){
    case 5:
      digitalWrite(CF1,HIGH);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
      statePD=0;
    break;
    case 9:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
      statePD=1;
    break;
    case 12:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,HIGH);
      statePD=2;
    break;
    case 15:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,HIGH);
      statePD=3;
    break;
    case 20:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,LOW);
      statePD=4;
    break;
  }
}

//cycle leds
void leds_cycle(){
  digitalWrite(LED1G,HIGH);
  digitalWrite(LED2G,HIGH);
  digitalWrite(LED1R,LOW);
  digitalWrite(LED2R,LOW);
  digitalWrite(LED1B,LOW);
  digitalWrite(LED2B,LOW);
  delay(500);
  digitalWrite(LED1G,LOW);
  digitalWrite(LED2G,LOW);
  digitalWrite(LED1R,HIGH);
  digitalWrite(LED2R,HIGH);
  digitalWrite(LED1B,LOW);
  digitalWrite(LED2B,LOW);
  delay(500);
  digitalWrite(LED1G,LOW);
  digitalWrite(LED2G,LOW);
  digitalWrite(LED1R,LOW);
  digitalWrite(LED2R,LOW);
  digitalWrite(LED1B,HIGH);
  digitalWrite(LED2B,HIGH);
  delay(500);
  digitalWrite(LED1G,LOW);
  digitalWrite(LED2G,LOW);
  digitalWrite(LED1R,LOW);
  digitalWrite(LED2R,LOW);
  digitalWrite(LED1B,LOW);
  digitalWrite(LED2B,LOW);
}



void IRAM_ATTR isrPage() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if(!flag_select_serial && !flag_select_power){
      a++;
      if(a>3){
        a=0;
      }
      attpage=true;
    }
    if (flag_select_serial){
      b++;
      if(b>4){
        b=0;
      }
      attpage3=true;
    }
    if (flag_select_power){
      c++;
      if(c>4){
        c=0;
      }
      attpage4=true;
    }
    if(flag_confirm){
      cansel=true;
    }
    last_button_time = button_time;
  }
}

void IRAM_ATTR isrENTER() {
  button_time3 = millis();
  if (button_time3 - last_button_time3 > 550)
  {
    if(flag_page3||flag_page4){
      enter=true;
    }
    if(flag_select_serial){
      attpage3=true;
    }
    if(flag_select_power){
      attpage4=true;
    }
    if(flag_confirm){
      confirm=true;
    }
    last_button_time3 = button_time3;
  }
}

void IRAM_ATTR isrDet(){
  button_time2 = millis();
  if (button_time2 - last_button_time2 > 250)
  {
    if(digitalRead(DET)==LOW){
      ext_com(true);
      if(V5&&PDvolt=="5V"){
      usbpd(5);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V9&&PDvolt=="9V"){
      usbpd(9);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V12&&PDvolt=="12V"){
      usbpd(12);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V15&&PDvolt=="15V"){
      usbpd(15);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V20&&PDvolt=="20V"){
      usbpd(20);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    }else{
      if(V12==HIGH){ 
        digitalWrite(POUT,LOW);      
        stateOUT=0;
        attfixo=true;
        digitalWrite(LED1B,LOW);
        digitalWrite(LED1G,LOW);
        digitalWrite(LED1R,HIGH);
        usbpd(5);
      } 
    }
    last_button_time2 = button_time2;
  }
}

void setup() {
  pins_init();
  digitalWrite(DPWR,HIGH);
  digitalWrite(LED1B,HIGH);
  if (!SPIFFS.begin(true)) {
    //handleSpiFFSError();
  }
  loaddata(BDR_data);
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  Serial1.begin(SBAUD.toInt(), SERIAL_8N1, RX, TX);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(0, 10, bitmap_logo, 128, 43, 1);
  display.display();
  test_pd();
  page_pd();
  delay(2000);
  if(digitalRead(DET)==LOW){
    ext_com(true);
    if(V5&&PDvolt=="5V"){
      usbpd(5);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V9&&PDvolt=="9V"){
      usbpd(9);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V12&&PDvolt=="12V"){
      usbpd(12);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V15&&PDvolt=="15V"){
      usbpd(15);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
    if(V20&&PDvolt=="20V"){
      usbpd(20);
      stateOUT=1;
      attfixo=true;
      digitalWrite(LED1B,LOW);
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH); 
    }
  }else{
    digitalWrite(LED1B,LOW);
    digitalWrite(LED1R,HIGH);
    digitalWrite(LED1G,LOW);
  }
  attachInterrupt(PAGE, isrPage, RISING);
  attachInterrupt(ENTER, isrENTER, RISING);
  attachInterrupt(DET, isrDet, CHANGE);
  delay(100);
  if(MODE == "Local"){
    mode_local();
  }
  if(MODE == "Cliente"){
    mode_cliente();
  }
  if(MODE == "Source"){
    mode_source();
  }


}

void loop() {
 
}


void sta_wifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(cWIFI, cWPAS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED&&flag_erro==false) {
    delay(100);
    switch(WiFi.status()){
        case WL_CONNECTED:
          ERRW = "0";  // Conectado com sucesso
        break;
        case WL_NO_SSID_AVAIL:
          ERRW = "1";  // Rede não encontrada // SSID inválido  
        break;
      }
    if(i>50){
      flag_erro = true;
    }
    i++;
  }
  if(ERRW!="1"&&flag_erro==true) {
    ERRW = "2"; // Senha incorreta
  }  
  if (flag_erro&&ERRW=="1") {
    // lida com o erro 1 // Rede não encontrada // SSID inválido
    Serial.println("rede nao encontrada");
    while(true){}
  }
  else if (flag_erro&&ERRW=="2") {
    // lida com o erro 2 // senha incorreta
    Serial.println("senha da rede incorreta");
    while(true){}
  }
  Serial.println("conectado a rede wifi");
  Serial.println(WiFi.localIP());
  int iPORT = strtol(PORT.c_str(), NULL, 0);
}

void mode_source(){
  sta_wifi();
  Serial.println("Subscribing MountPoint");
  if (!ntrip_s.subStation(host, httpPort, mntpnt,psw,srcSTR)) {
    // lida erro de conexão
  } else {
    Serial.println("Subscribing MountPoint is OK");
  }
  while(true){
    if (ntrip_s.connected()) {
      while (Serial2.available()) {
        readcount = 0;
        while (Serial2.available()) {
          ch[readcount] = Serial2.read();
          readcount++;
          if (readcount > 511)break;
        }//buffering
        ntrip_s.write((uint8_t*)ch, readcount);
      }
    }
    else {
      ntrip_s.stop();
      Serial.println("reconnect");
      Serial.println("Subscribing MountPoint");
      if (!ntrip_s.subStation(host, httpPort, mntpnt, psw, srcSTR)) {
        delay(100);
      }
      else {
        Serial.println("Subscribing MountPoint is OK");
        delay(10);
      }

    }
    delay(10);  //server cycle
  }
}

void mode_cliente(){
  sta_wifi();
  Serial.println("Requesting SourceTable.");
  switch(ntrip_c.reqSrcTbl(cHOST,iPORT)){
    case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(cHOST);Serial.print(":");Serial.println(iPORT);
            Serial.println("SourceTable request error");
    break;
    case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
            Serial.println("SourceTable request error");
    break;
    case 4: Serial.print("tudo ok!");
            char buffer[512];
            delay(1000);
            while(ntrip_c.available()){
              ntrip_c.readLine(buffer,sizeof(buffer));
              Serial.print(buffer); 
            }
            Serial.println("Requesting SourceTable is OK\n");
    break;
    case 5: Serial.println("Servidor não responde");
            Serial.println("SourceTable request error");
    break;
    case 6: Serial.println("Mount Point incorreto");
    break;
  }
  ntrip_c.stop(); //Need to call "stop" function for next request.
  Serial.println("Requesting MountPoint's Raw data");     
  if(ChLoc=="S"){
    if(Latitude!=""&&Longitude!=""&&Altitude!=""&&Tempoutc!=""){
      String nmeaGGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
      switch(ntrip_c.reqRaw(cHOST,iPORT,cMNTP,cUSER,cUPAS,nmeaGGA,ChLoc)){
        case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(cHOST);Serial.print(":");Serial.println(iPORT);
                // lida com o erro
                ntrip_c.stop();
        break;
        case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
                // lida com o erro
                ntrip_c.stop();
        break;
        case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(cUSER);Serial.print(":");Serial.println(cUPAS);
                Serial.println("unable to connect with ntrip caster");
                // lida com o erro
                ntrip_c.stop();
        break;
        case 4: Serial.print("tudo ok!");
                clientmodo = 1;
        break;
        case 5: Serial.println("Servidor não responde");
                // lida com o erro
                ntrip_c.stop();
        break;
        case 6: Serial.println("Mount Point incorreto");
                // lida com o erro
                ntrip_c.stop();
        break;
      }
    }
  }else if (ChLoc=="N"){
    switch(ntrip_c.reqRaw(cHOST,iPORT,cMNTP,cUSER,cUPAS,"","")){
      case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(cHOST);Serial.print(":");Serial.println(iPORT);
              // lida com o erro
                ntrip_c.stop();
      break;
      case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
              // lida com o erro
                ntrip_c.stop();
      break;
      case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(cUSER);Serial.print(":");Serial.println(cUPAS);
              Serial.println("unable to connect with ntrip caster");
              // lida com o erro
                ntrip_c.stop();
      break;
      case 4: Serial.print("tudo ok!");
              clientmodo = 1;
      break;
      case 5: Serial.println("Servidor não responde");
              // lida com o erro
                ntrip_c.stop();
      break;
      case 6: Serial.println("Mount Point incorreto");
              // lida com o erro
                ntrip_c.stop();
        break;
    }
  }
  // indicação LED 1
  input = 1;
  milli_inicio = millis();
  String GGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
  String enviaGGA = GGA + "\r\n";
  while(true){
    //Serial.println("while 1");
    if(ntrip_c.available()){
      currentMillis = millis();
      // indicação LED 2
      sai1:
      sai2:
      while(ntrip_c.available()){
        readcount = ntrip_c.readBytes(ch0, ntrip_c.available());
        Serial1.write(ch0, readcount);
        //Serial.println(ch0);
        if(ntrip_c.available()==0){
          Serial.println(readcount);
          readcount = 0;
          goto sai1;
        }
        if(digitalRead(BT1)==1){
          readcount = 0;
          goto sai2;
        }
      }
      
    }
    atualMillis = millis();
    if(atualMillis - milli_inicio > 20000){  
      ntrip_c.sendgga(enviaGGA);
      milli_inicio = millis();
    }
    
    if(atualMillis - currentMillis > 30000){
      // lida com erro timeout
    }
    delay(1);
    // indica led 2
    //att display
  }
}

void mode_local(){
  WiFi.softAP("BDR-", NULL, 7, 0, 5);
  String macAddress = WiFi.softAPmacAddress();
  String lastFourDigits = macAddress.substring(macAddress.length() - 5);
  macAddress.replace(":", "");
  password = macAddress.substring(macAddress.length() - 8);
  ssid = "BDR-" + lastFourDigits;
  WiFi.softAP(ssid, password);
  wifiServer.begin();
  fixo();
  page1();
  while(true){
    WiFiClient client = wifiServer.available();
    
    if (client && !pausa) {
  
      while (client.connected()) {
        int counter = 0;
        attDisplay();
        
        while (client.available()>0) {
          cliente_data[counter] = client.read();
          counter++;
          if(client.available()==0){        
            
            String c_data_s = String(cliente_data);

            if(c_data_s.indexOf("$GPGGA")==-1){
              
              currentMillis = millis();
              switch(check_ver(c_data_s)*check_mountpoint(c_data_s,mountpoint)*check_aut(c_data_s,usuario,senha)){
                case 40:  Serial.println("REV1 + mount point correto + autentificação correta");// REV1 + mount point correto + autentificação correta
                          client.println(res_rev1);
                          flag_ok = true;
                          digitalWrite(LED2B,LOW);
                          digitalWrite(LED2G,HIGH);
                          digitalWrite(LED2R,LOW);
                  break;
                case 48:  Serial.println("REV1 + solicita mount point + autentificação correta");// REV1 + solicita mount point + autentificação correta
                          client.println(res_rev1_ST);
                          client.println(scrtbl(mountpoint,WiFi.softAPIP().toString(),port));
                          client.stop();
                  break;
                case 56:  Serial.println("REV1 + mount point incorreto + autentificação correta");// REV1 + mount point incorreto + autentificação correta
                          flag_ok = false;
                  break;
                case 45:  Serial.println("REV1 + mount point correto + autentificação incorreta");// REV1 + mount point correto + autentificação incorreta
                          flag_ok = false;
                  break;
                case 54:  Serial.println("REV1 + solicita mount point + autentificação incorreta");// REV1 + solicita mount point + autentificação incorreta
                          flag_ok = false;
                  break; 
                case 63:  Serial.println("REV1 + mount point incorreto + autentificação incorreta");// REV1 + mount point incorreto + autentificação incorreta
                          flag_ok = false;
                  break;

                case 80:  Serial.println("REV2 + mount point correto + autentificação correta");// REV2 + mount point correto + autentificação correta
                          client.println(res_rev1);
                          flag_ok = true;
                          digitalWrite(LED2B,LOW);
                          digitalWrite(LED2G,HIGH);
                          digitalWrite(LED2R,LOW);
                  break;
                case 96:  Serial.println("REV2 + solicita mount point + autentificação correta");// REV2 + solicita mount point + autentificação correta
                          client.println(res_rev1_ST);
                          client.println(scrtbl(mountpoint,WiFi.softAPIP().toString(),port));
                          client.stop();
                  break;
                case 112: Serial.println("REV2 + mount point incorreto + autentificação correta");// REV2 + mount point incorreto + autentificação correta
                          flag_ok = false;
                  break;
                case 90:  Serial.println("REV2 + mount point correto + autentificação incorreta");// REV2 + mount point correto + autentificação incorreta
                          flag_ok = false;
                  break;
                case 108: Serial.println("REV2 + solicita mount point + autentificação incorreta");// REV2 + solicita mount point + autentificação incorreta
                          flag_ok = false;
                  break;
                case 126: Serial.println("REV2 + mount point incorreto + autentificação incorreta");// REV2 + mount point incorreto + autentificação incorreta
                          flag_ok = false;
                  break;

                case 160: Serial.println("NAO HTTP + mount point correto + autentificação correta");// NAO HTTP + mount point correto + autentificação correta
                          flag_ok = false;
                  break;
                case 192: Serial.println("NAO HTTP + solicita mount point + autentificação correta");// NAO HTTP + solicita mount point + autentificação correta
                          flag_ok = false;
                  break;
                case 224: Serial.println("NAO HTTP + mount point incorreto + autentificação correta");// NAO HTTP + mount point incorreto + autentificação correta
                          flag_ok = false;
                  break;
                case 180: Serial.println("NAO HTTP + mount point correto + autentificação incorreta");// NAO HTTP + mount point correto + autentificação incorreta
                          flag_ok = false;
                  break;
                case 216: Serial.println("NAO HTTP + solicita mount point + autentificação incorreta");// NAO HTTP + solicita mount point + autentificação incorreta
                          flag_ok = false;
                  break;
                case 252: Serial.println("NAO HTTP + mount point incorreto + autentificação incorreta");// NAO HTTP + mount point incorreto + autentificação incorreta
                          flag_ok = false;
                  break;
              }
              long int size = sizeof(cliente_data);
              memset(cliente_data,' ',size*sizeof(char));
            }
            else{
              currentMillis = millis();
              flag_send=true;
            }
          } 
        }
          atualMillis = millis();
          if(atualMillis - currentMillis > 2500 && flag_ok == true && currentMillis != 0 && flag_send==true){
            client.stop();
            Serial.println(atualMillis - currentMillis);
            currentMillis = 0;
            atualMillis = 0;
            Serial.println("Client disconnected");
            
            long int size = sizeof(cliente_data);
            memset(cliente_data,' ',size*sizeof(char));
            digitalWrite(LED2B,LOW);
            digitalWrite(LED2G,LOW);
            digitalWrite(LED2R,HIGH);
            delay(200);
            flag_send=false;
            //ESP.restart();
        }

        while (Serial1.available() && flag_ok == true) {
          digitalWrite(LED2B,LOW);
          digitalWrite(LED2G,HIGH);
          digitalWrite(LED2R,LOW);
          readcount = 0;
          while (Serial1.available()) {
            ch[readcount] = Serial1.read();
            readcount++;
            if (readcount > 511)break;
          }//buffering
          client.write((uint8_t*)ch, readcount);
          digitalWrite(LED2B,LOW);
          digitalWrite(LED2G,LOW);
          digitalWrite(LED2R,LOW);
        }
      }
      client.stop();
      Serial.println("Client disconnected");
      flag_ok == false;
      long int size = sizeof(cliente_data);
      currentMillis = 0;
      atualMillis = 0;
      memset(cliente_data,' ',size*sizeof(char));
      digitalWrite(LED2B,LOW);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2R,HIGH);
      flag_send=false;
    }
    attDisplay();
  }
}


