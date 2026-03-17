#include "WiFi.h"
#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "HardwareSerial.h"
#include "base64.hpp"
#include "NTPClient.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "ESPAsyncWebServer.h"
#include "DNSServer.h"
#include "AsyncTCP.h"

#define BDR_data  "/BDR.json"

#define size_bdr  2048

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


const uint8_t bitmap_wifi[] = {0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x06, 0x00, 0x73, 0x00, 0x79, 0x00, 0x0d, 0x80, 0x6d, 0x80, 0x6d, 0x80, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};

String ssid;
String password;
String SBAUD = "115200";
const char* mountpoint = "BDR0";
const char* usuario = "HGNP";
const char* senha  = "0102";
uint16_t port = 2101;
String Conf; char cConf[2];

//GERAL
String Modo, Perfil, Mac;
char cModo[10]; char cPerfil[10]; char cMac[10];

//LOCAL
//portaSupL
String saidaTenL, baudrateL, paridadeL, bitsParaL, contrFluxL, datbitL;
char csaidaTenL[10]; char cbaudrateL[10]; char cparidadeL[10]; char cbitsParaL[10]; char ccontrFluxL[10]; char cdatbitL[10]; 

//wifiL
String ssidL, senwiL;
char cssidL[10]; char csenwiL[10];

//ntripConfL
String hostNtpL, portNtpL, mtpntL, userL, senL;
char chostNtpL[10]; char cportNtpL[10], cmtpntL[10]; char cuserL[10]; char csenL[10];
//LOCAL

//CLIENTE
//portaSupC
String saidaTenC, baudrateC, paridadeC, bitsParaC, contrFluxC, datbitC;
char csaidaTenC[10]; char cbaudrateC[10]; char cparidadeC[10]; char cbitsParaC[10]; char ccontrFluxC[10]; char cdatbitC[10]; 

//wifiC
String ssidC, senwiC;
char cssidC[10]; char csenwiC[10];

//ntripConfL
String hostNtpC, portNtpC, mtpntC, userC, senC;
char chostNtpC[10]; char cportNtpC[10], cmtpntC[10]; char cuserC[10]; char csenC[10];

//latLong
String sendLatLong, lat, lon, alt, pre, utc;
char csendLatLong[10]; char clat[10], clon[10]; char calt[10]; char cpre[10]; char cutc[10];
//CLIENTE

//SOURCE
//portaSupS
String saidaTenS, baudrateS, paridadeS, bitsParaS, contrFluxS, datbitS;
char csaidaTenS[10]; char cbaudrateS[10]; char cparidadeS[10]; char cbitsParaS[10]; char ccontrFluxS[10]; char cdatbitS[10]; 

//wifiS
String ssidS, senwiS;
char cssidS[10]; char csenwiS[10];

//ntripConfS
String hostNtpS, portNtpS, mtpntS, sendRev2, userS, senS;
char chostNtpS[10]; char cportNtpS[10], cmtpntS[10]; char csendRev2[10]; char cuserS[10]; char csenS[10];
//SOURCE

AsyncWebServer server(80);

const char* res_rev1 = "ICY 200 OK";
const char* res_rev2 = "HTTP/1.1 200 OK\r\n";
const char* res_rev1_ST = "SOURCETABLE 200 OK";

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
String volt[5]={"5V","9V","12V","15V","20V"};
String PDvolt = "12V";
String OldPDvolt;
char cJSBAUD[7]; char cJVSET [4];


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
      if(filename=="/BDR.json"){
        File configFile = SPIFFS.open(filename, "r");
          if (configFile){
            StaticJsonDocument<size_bdr> json;
            DeserializationError error = deserializeJson(json, configFile);
            if (!error){
              //LOCAL
              //portaSupL
              JsonObject portaSupL = json["portaSupL"][0];
              saidaTenL = strcpy(csaidaTenL, portaSupL["saidaTenL"]);
              baudrateL = strcpy(cbaudrateL, portaSupL["baudrateL"]);
              paridadeL = strcpy(cparidadeL, portaSupL["paridadeL"]);
              bitsParaL = strcpy(cbitsParaL, portaSupL["bitsParaL"]);
              contrFluxL = strcpy(ccontrFluxL, portaSupL["contrFluxL"]);
              datbitL = strcpy(cdatbitL, portaSupL["datbitL"]);
              //wifiL
              JsonObject wifiL = json["wifiL"][0];
              ssidL = strcpy(cssidL, wifiL["ssidL"]);
              senwiL = strcpy(csenwiL, wifiL["senwiL"]);
              //ntripConfL
              JsonObject ntripConfL = json["ntripConfL"][0];
              hostNtpL = strcpy(chostNtpL, ntripConfL["hostNtpL"]);
              portNtpL = strcpy(cportNtpL, ntripConfL["portNtpL"]);
              mtpntL = strcpy(cmtpntL, ntripConfL["mtpntL"]);
              userL = strcpy(cuserL, ntripConfL["userL"]);
              senL = strcpy(csenL, ntripConfL["senL"]);
              //CLIENTE
              //portaSupC
              JsonObject portaSupC = json["portaSupC"][0];
              saidaTenC = strcpy(csaidaTenC, portaSupC["saidaTenC"]);
              baudrateC = strcpy(cbaudrateC, portaSupC["baudrateC"]);
              paridadeC = strcpy(cparidadeC, portaSupC["paridadeC"]);
              bitsParaC = strcpy(cbitsParaC, portaSupC["bitsParaC"]);
              contrFluxC = strcpy(ccontrFluxC, portaSupC["contrFluxC"]);
              datbitC = strcpy(cdatbitC, portaSupC["datbitC"]);
              //wifiC
              JsonObject wifiC = json["wifiC"][0];
              ssidC = strcpy(cssidC, wifiC["ssidC"]);
              senwiC = strcpy(csenwiC, wifiC["senwiC"]);
              //ntripConfC
              JsonObject ntripConfC = json["ntripConfC"][0];
              hostNtpC = strcpy(chostNtpC, ntripConfC["hostNtpC"]);
              portNtpC = strcpy(cportNtpC, ntripConfC["portNtpC"]);
              mtpntC = strcpy(cmtpntC, ntripConfC["mtpntC"]);
              userC = strcpy(cuserC, ntripConfC["userC"]);
              senC = strcpy(csenC, ntripConfC["senC"]);
              //latLong
              JsonObject latLong = json["latLong"][0];
              sendLatLong = strcpy(csendLatLong, latLong["sendLatLong"]);
              lat = strcpy(clat, latLong["lat"]);
              lon = strcpy(clon, latLong["lon"]);
              alt = strcpy(calt, latLong["alt"]);
              pre = strcpy(cpre, latLong["pre"]);
              utc = strcpy(cutc, latLong["utc"]);
              //SOURCE
              //portaSupS
              JsonObject portaSupS = json["portaSupS"][0];
              saidaTenS = strcpy(csaidaTenS, portaSupS["saidaTenS"]);
              baudrateS = strcpy(cbaudrateS, portaSupS["baudrateS"]);
              paridadeS = strcpy(cparidadeS, portaSupS["paridadeS"]);
              bitsParaS = strcpy(cbitsParaS, portaSupS["bitsParaS"]);
              contrFluxS = strcpy(ccontrFluxS, portaSupS["contrFluxS"]);
              datbitS = strcpy(cdatbitS, portaSupS["datbitS"]);
              //wifiS
              JsonObject wifiS = json["wifiS"][0];
              ssidS = strcpy(cssidS, wifiS["ssidS"]);
              senwiS = strcpy(csenwiS, wifiS["senwiS"]);
              //ntripConfS
              JsonObject ntripConfS = json["ntripConfS"][0];
              hostNtpS = strcpy(chostNtpS, ntripConfS["hostNtpS"]);
              portNtpS = strcpy(cportNtpS, ntripConfS["portNtpS"]);
              mtpntS = strcpy(cmtpntS, ntripConfS["mtpntS"]);
              sendRev2 = strcpy(csendRev2, ntripConfS["sendRev2"]);
              userS = strcpy(cuserS, ntripConfS["userS"]);
              senS = strcpy(csenS, ntripConfS["senS"]);
              JsonObject avancado = json["avancado"][0];
              Modo = strcpy(cModo, avancado["Modo"]);
              Perfil = strcpy(cPerfil, avancado["Perfil"]);
              Mac = strcpy(cMac, avancado["Mac"]);
              Conf = strcpy(cConf, avancado["Conf"]);
              return true;
          }else{
              Serial.println("Failed to load bdrjson.json");
          }
        }else{
          Serial.println("Failed to load bdrjson.json");
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
  if(filename=="/BDR.json"){
    StaticJsonDocument<size_bdr> json;
    //LOCAL
    JsonObject portaSupL = json["portaSupL"].createNestedObject();
    portaSupL["saidaTenL"] = saidaTenL.c_str();
    portaSupL["baudrateL"] = baudrateL.c_str();
    portaSupL["paridadeL"] = paridadeL.c_str();
    portaSupL["bitsParaL"] = bitsParaL.c_str();
    portaSupL["contrFluxL"] = contrFluxL.c_str();
    portaSupL["datbitL"] = datbitL.c_str();
    JsonObject wifiL = json["wifiL"].createNestedObject();
    wifiL["ssidL"] = ssidL.c_str();
    wifiL["senwiL"] = senwiL.c_str();
    JsonObject ntripConfL = json["ntripConfL"].createNestedObject();
    ntripConfL["hostNtpL"] = hostNtpL.c_str();
    ntripConfL["portNtpL"] = portNtpL.c_str();
    ntripConfL["mtpntL"] = mtpntL.c_str();
    ntripConfL["userL"] = userL.c_str();
    ntripConfL["senL"] = senL.c_str();
    //CLIENTE
    JsonObject portaSupC = json["portaSupC"].createNestedObject();
    portaSupC["saidaTenC"] = saidaTenC.c_str();
    portaSupC["baudrateC"] = baudrateC.c_str();
    portaSupC["paridadeC"] = paridadeC.c_str();
    portaSupC["bitsParaC"] = bitsParaC.c_str();
    portaSupC["contrFluxC"] = contrFluxC.c_str();
    portaSupC["datbitC"] = datbitC.c_str();
    JsonObject wifiC = json["wifiC"].createNestedObject();
    wifiC["ssidC"] = ssidC.c_str();
    wifiC["senwiC"] = senwiC.c_str();
    JsonObject ntripConfC = json["ntripConfC"].createNestedObject();
    ntripConfC["hostNtpC"] = hostNtpC.c_str();
    ntripConfC["portNtpC"] = portNtpC.c_str();
    ntripConfC["mtpntC"] = mtpntC.c_str();
    ntripConfC["userC"] = userC.c_str();
    ntripConfC["senC"] = senC.c_str();
    JsonObject latLong = json["latLong"].createNestedObject();
    latLong["sendLatLong"] = sendLatLong.c_str();
    latLong["lat"] = lat.c_str();
    latLong["lon"] = lon.c_str();
    latLong["alt"] = alt.c_str();
    latLong["pre"] = pre.c_str();
    latLong["utc"] = utc.c_str();
    //SOURCE
    JsonObject portaSupS = json["portaSupS"].createNestedObject();
    portaSupS["saidaTenS"] = saidaTenS.c_str();
    portaSupS["baudrateS"] = baudrateS.c_str();
    portaSupS["paridadeS"] = paridadeS.c_str();
    portaSupS["bitsParaS"] = bitsParaS.c_str();
    portaSupS["contrFluxS"] = contrFluxS.c_str();
    portaSupS["datbitS"] = datbitS.c_str();
    JsonObject wifiS = json["wifiS"].createNestedObject();
    wifiS["ssidS"] = ssidS.c_str();
    wifiS["senwiS"] = senwiS.c_str();
    JsonObject ntripConfS = json["ntripConfS"].createNestedObject();
    ntripConfS["hostNtpS"] = hostNtpS.c_str();
    ntripConfS["portNtpS"] = portNtpS.c_str();
    ntripConfS["mtpntS"] = mtpntS.c_str();
    ntripConfS["sendRev2"] = sendRev2.c_str();
    ntripConfS["userS"] = userS.c_str();
    ntripConfS["senS"] = senS.c_str();
    JsonObject avancado = json["avancado"].createNestedObject();
    avancado["Modo"] = Modo.c_str();
    avancado["Perfil"] = Perfil.c_str();
    avancado["Mac"] = Mac.c_str();
    avancado["Conf"] = Conf.c_str();
    File configFile = SPIFFS.open(filename, "w");
    if (!configFile){
      Serial.println("failed to open bdrjson.json file for writing");
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

void fixo() {
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("PWR:");
    switch(statePD){
      case 0: display.print("5V");
      break;
      case 1: display.print("9V");
      break;
      case 2: display.print("12V");
      break;
      case 3: display.print("15V");
      break;
      case 4: display.print("20V");
      break;
    }
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
    display.print("PWR:");
    switch(statePD){
      case 0: display.print("5V");
      break;
      case 1: display.print("9V");
      break;
      case 2: display.print("12V");
      break;
      case 3: display.print("15V");
      break;
      case 4: display.print("20V");
      break;
    }
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
    display.print("PWR:");
    switch(statePD){
      case 0: display.print("5V");
      break;
      case 1: display.print("9V");
      break;
      case 2: display.print("12V");
      break;
      case 3: display.print("15V");
      break;
      case 4: display.print("20V");
      break;
    }
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
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("NTRIPHost:");
    display.print(WiFi.softAPIP());
    display.setCursor(0, 22);
    display.print("Port:");
    display.print(port);
    display.setCursor(65, 22);
    display.print("User:");
    display.print(usuario);
    display.setCursor(0, 32);
    display.print("Password:");
    display.print(senha);
    display.setCursor(0, 42);
    display.print("MountPoint:");
    display.print(mountpoint);
    display.display();
}

void page3() {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.setCursor(66, 22);
    display.print(SBAUD);
    display.setCursor(0, 32);
    display.print("Parity:None    Data:8");
    display.setCursor(0, 42);
    display.print("Stop: 1     Flow:None");
    display.display();
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
    display.print("Parity:None    Data:8");
    display.setCursor(0, 42);
    display.print("Stop: 1     Flow:None");
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

void test_pd(){
  usbpd(9);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V9=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V9=LOW;
  }
  usbpd(12);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V12=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V12=LOW;
  }
  usbpd(15);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V15=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V15=LOW;
  }
  usbpd(20);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V20=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V20=LOW;
  }
  if(V9==LOW&&V12==LOW&&V15==LOW&&V20==LOW){
    stateOUT=2;
  }
  usbpd(5);
  V5=HIGH;
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


String scrtbl(const char* mountpoint, String ip, uint16_t port){
  String body = "STR;";
  body = body + mountpoint + String(";;;;0;;HUGEN;BRA;0.00;0.00;0;0;sNTRIP;none;N;N;0;none;\r\n"
            "NET;HUGEN;;N;N;None;") + ip + String(":") + port + String(";None;;\r\n"
            "ENDSOURCETABLE\r\n");
  uint16_t tamanho = body.length();
  String header = "Server:"; 
  header = header + String(" Hugenplus ntrip simple ntrip caster/R1.0\r\n"
            //"Date: ") + ntp.getFormattedDate() + String("\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: ") + String(tamanho) + String("\r\n");
  return header + String("\r\n") + body + String("\r\n");
}

int check_aut(String data, const char* user, const char* pwr){
  char base64[50];
  unsigned char string[50];

  int posicao = data.indexOf(": Basic");
  posicao = posicao + 8;
  String usuario_encoded = data.substring(posicao);
  usuario_encoded.toCharArray(base64,50);
  unsigned int string_length = decode_base64((unsigned char *)base64, string);
  string[string_length] = '\0';
  String usuario_decoded = String((char *)string);
  int separador = usuario_decoded.indexOf(":");
  String cliente_usuario = usuario_decoded.substring(0,separador);
  separador ++;
  String cliente_senha = usuario_decoded.substring(separador);
  if(cliente_usuario.equals(usuario)&cliente_senha.equals(senha)){
    return 8;// senha correta
  }else{
    return 9;// senha incorreta
  }
}

int check_ver(String data){
  int posicao_inicial = data.indexOf("HTTP");
  int posicao_final = data.indexOf("\r\n");
  if (posicao_inicial < 0){
    return 4; //ERRO1 
  }
  String versao_http = data.substring(posicao_inicial, posicao_final);
  //Serial.println(versao_http);
  if(versao_http == "HTTP/1.0"){
    return 1; //REV1
  }
  if(versao_http == "HTTP/1.1"){
    return 2; //REV2
  }
}

int check_mountpoint(String data, const char* mountpoint){
  int posicao_inicial = data.indexOf("/");
  int posicao_final = data.indexOf("HTTP");
  posicao_final--;
  String cliente_mountpoint = data.substring(posicao_inicial,posicao_final);
  String mp = "/";
  mp = mp + mountpoint;
  //Serial.println(cliente_mountpoint);
  //Serial.println(mp);
  if(cliente_mountpoint == mp){
    return 5; //igual
  }
  if(cliente_mountpoint == "/SOURCETABLE.TXT " || cliente_mountpoint == "/"){
    return 6; //solicita
  }
  return 7; //ERRO2
} 

void attDisplay(){
  devices=WiFi.softAPgetStationNum();
  if(devices!=devicesOld){
    attpage=true;
    devicesOld=devices;
  }
  if(attpage==true){
    delay(300);
    switch(a){
      case 0: page1();
              flag_page3=false;
              flag_page4=false;
      break;
      case 1: page2();
              flag_page3=false;
              flag_page4=false;
      break;
      case 2: page3();
              flag_page3=true;
              flag_page4=false;
      break;
      case 3: page4();
              flag_page3=false;
              flag_page4=true;
      break;
    }
    attpage=false;
  }
  if(flag_page3&&enter&& !flag_select_serial){
    for(int q = 0; q<5 ; q++){
      if(BAUDS[q]==SBAUD){
        b = q;
      }
    }
    flag_select_serial = true;
    attfixo=true;
    attpage3=true;
    pausa=true;
    enter=false;
  }
  if(flag_page4 && enter && !flag_select_power){
    OldPDvolt = PDvolt;
    for(int w = 0; w<5; w++){
      if(volt[w]==PDvolt){
        c = w;
      }
    }
    flag_select_power=true;
    attfixo=true;
    attpage4=true;
    pausa=true;
    enter=false;
  }
  if(attpage3==true){
    page3select(BAUDS[b]);
    if(enter && flag_select_serial){
      SBAUD = BAUDS[b];
      saveConfigFile(BDR_data);
      Serial1.begin(SBAUD.toInt(), SERIAL_8N1, RX, TX);
      attfixo=true;
      flag_select_serial=false;
      pausa=false;
      enter=false;
    }
    attpage3=false;
  }
  if(attpage4==true){
    page4select(volt[c]);
    if(enter && flag_select_power){
      if(!flag_confirm){
        page4confirm(volt[c]);
        fixoConfirm();
      }
      flag_confirm=true;
      if(enter && flag_select_power && confirm){
        PDvolt = volt[c];
        saveConfigFile(BDR_data);
        page4();
        attfixo = true;
        flag_select_power = false;
        pausa=false;
        enter=false;
        confirm=false;
        flag_confirm=false;
        if(digitalRead(DET)==LOW){
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
        }
      }
      if(enter && flag_select_power && cansel){
        PDvolt = OldPDvolt;
        saveConfigFile(BDR_data);
        page4();
        attfixo = true;
        flag_select_power = false;
        pausa=false;
        enter=false;
        cansel=false;
        flag_confirm=false;
      }
    }
    attpage4=false;
  }
  if(attfixo==true){
    delay(300);
    if(flag_select_serial){
      fixo2();
    }else if (flag_select_power){
      fixo2();
    }else{
      fixo();
    } 
    attfixo=false;
    if(stateOUT==1){
      digitalWrite(POUT,HIGH);
    }
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

  if (Conf == "1") {
      WiFi.softAP("BDR-", NULL, 7, 0, 5);
      String macAddress = WiFi.softAPmacAddress();
      String lastFourDigits = macAddress.substring(macAddress.length() - 5);
      macAddress.replace(":", "");
      password = macAddress.substring(macAddress.length() - 8);
      ssid = ("BDR-" + lastFourDigits);
      WiFi.softAP(ssid, password);
      Mac = lastFourDigits;
      saveConfigFile(BDR_data);
      Serial.println("senha é: " + password);
      IPAddress Ip(192, 168, 0, 1);
      IPAddress Iplocal(192, 168, 0, 1);
      IPAddress NMask(255, 255, 255, 0);
      WiFi.softAPConfig(Iplocal, Ip, NMask);
    
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(SPIFFS, "/", String(), false);
        });
        server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(204);
        });
        server.on("/BDR.json", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(SPIFFS, "/BDR.json", "application/json");
          Serial.println("Json Enviado");
        });
        server.on("/localbdr", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
        if (request->hasParam("saidaTenL", true)) {
            AsyncWebParameter* aa = request->getParam("saidaTenL", true);
            AsyncWebParameter* ab = request->getParam("baudrateL", true); 
            AsyncWebParameter* ac = request->getParam("paridadeL", true); 
            AsyncWebParameter* ad = request->getParam("bitsParaL", true); 
            AsyncWebParameter* ae = request->getParam("contrFluxL", true); 
            AsyncWebParameter* af = request->getParam("datbitL", true);
            AsyncWebParameter* ag = request->getParam("ssidL", true); 
            AsyncWebParameter* ah = request->getParam("senwiL", true);
            AsyncWebParameter* ai = request->getParam("hostNtpL", true); 
            AsyncWebParameter* aj = request->getParam("portNtpL", true); 
            AsyncWebParameter* ak = request->getParam("mtpntL", true); 
            AsyncWebParameter* al = request->getParam("userL", true); 
            AsyncWebParameter* am = request->getParam("senL", true); 
            AsyncWebParameter* an = request->getParam("Modo", true); 
            AsyncWebParameter* ao = request->getParam("Perfil", true); 
    
            saidaTenL = aa->value();
            baudrateL = ab->value();
            paridadeL = ac->value();
            bitsParaL = ad->value();
            contrFluxL = ae->value();
            datbitL = af->value();
            ssidL = ag->value();
            senwiL = ah->value();
            hostNtpL = ai->value();
            portNtpL = aj->value();
            mtpntL = ak->value();
            userL = al->value();
            senL = am->value();
            Modo = an->value();
            Perfil = ao->value();
            }
            Conf = "0";
            saveConfigFile(BDR_data);
            request->send(200);
            ESP.restart();
        });
        server.on("/clientebdr", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
        if (request->hasParam("saidaTenC", true)) {
            AsyncWebParameter* ba = request->getParam("saidaTenC", true);
            AsyncWebParameter* bb = request->getParam("baudrateC", true); 
            AsyncWebParameter* bc = request->getParam("paridadeC", true); 
            AsyncWebParameter* bd = request->getParam("bitsParaC", true); 
            AsyncWebParameter* be = request->getParam("contrFluxC", true); 
            AsyncWebParameter* bf = request->getParam("datbitC", true);
            AsyncWebParameter* bg = request->getParam("ssidC", true); 
            AsyncWebParameter* bh = request->getParam("senwiC", true);
            AsyncWebParameter* bi = request->getParam("hostNtpC", true); 
            AsyncWebParameter* bj = request->getParam("portNtpC", true); 
            AsyncWebParameter* bk = request->getParam("mtpntC", true); 
            AsyncWebParameter* bl = request->getParam("userC", true); 
            AsyncWebParameter* bm = request->getParam("senC", true); 
            AsyncWebParameter* bn = request->getParam("sendLatLong", true); 
            AsyncWebParameter* bo = request->getParam("lat", true); 
            AsyncWebParameter* bp = request->getParam("lon", true); 
            AsyncWebParameter* bq = request->getParam("alt", true); 
            AsyncWebParameter* br = request->getParam("pre", true); 
            AsyncWebParameter* bs = request->getParam("utc", true);
            AsyncWebParameter* bt = request->getParam("Modo", true);
            AsyncWebParameter* bu = request->getParam("Perfil", true);
            
            saidaTenC = ba->value();
            baudrateC = bb->value();
            paridadeC = bc->value();
            bitsParaC = bd->value();
            contrFluxC = be->value();
            datbitC = bf->value();
            ssidC = bg->value();
            senwiC = bh->value();
            hostNtpC = bi->value();
            portNtpC = bj->value();
            mtpntC = bk->value();
            userC = bl->value();
            senC = bm->value();
            sendLatLong = bn->value();
            lat = bo->value();
            lon = bp->value();
            alt = bq->value();
            pre = br->value();
            utc = bs->value();
            Modo = bt->value();
            Perfil = bu->value();
            }
            saveConfigFile(BDR_data);
            request->send(200);
        });
        server.on("/sourcebdr", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
        if (request->hasParam("saidaTenS", true)) {
            AsyncWebParameter* ca = request->getParam("saidaTenS", true);
            AsyncWebParameter* cb = request->getParam("baudrateS", true); 
            AsyncWebParameter* cc = request->getParam("paridadeS", true); 
            AsyncWebParameter* cd = request->getParam("bitsParaS", true); 
            AsyncWebParameter* ce = request->getParam("contrFluxS", true); 
            AsyncWebParameter* cf = request->getParam("datbitS", true);
            AsyncWebParameter* cg = request->getParam("ssidS", true); 
            AsyncWebParameter* ch = request->getParam("senwiS", true);
            AsyncWebParameter* ci = request->getParam("hostNtpS", true); 
            AsyncWebParameter* cj = request->getParam("portNtpS", true); 
            AsyncWebParameter* ck = request->getParam("mtpntS", true);
            AsyncWebParameter* cl = request->getParam("sendRev2", true); 
            AsyncWebParameter* cm = request->getParam("userS", true); 
            AsyncWebParameter* cn = request->getParam("senS", true);
            AsyncWebParameter* co = request->getParam("Modo", true); 
            AsyncWebParameter* cp = request->getParam("Perfil", true); 
            
            saidaTenS = ca->value();
            baudrateS = cb->value();
            paridadeS = cc->value();
            bitsParaS = cd->value();
            contrFluxS = ce->value();
            datbitS = cf->value();
            ssidS = cg->value();
            senwiS = ch->value();
            hostNtpS = ci->value();
            portNtpS = cj->value();
            mtpntS = ck->value();
            sendRev2 = cl->value();
            userS = cm->value();
            senS = cn->value();
            Modo = co->value();
            Perfil = cp->value();
            }
            saveConfigFile(BDR_data);
            request->send(200);
        });
        server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
            saveConfigFile(BDR_data);
            request->send(200);
        });
    
      server.begin();
      wifiServer.begin();
      test_pd();
      page_pd();
      delay(2000);
      fixo();
      page1();
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
    } else if (Conf == 0 && Modo == "Local") {
      local();
    }
}

void local() {
  Serial.println("Local Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidL, senwiL, 7, 0, 10);
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  wifiServer.begin();
  Serial.print("IP: ");Serial.println(WiFi.softAPIP());
  WiFiClient client = wifiServer.available();
}

//void cliente() {
  
//}

//void source() {
  
//}

void loop() {
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
