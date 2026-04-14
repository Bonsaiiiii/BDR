#include "WiFi.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"

#define DEBUG 1
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
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DSP_ADDR 0x3c
#define OLED_RESET     DRST // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS DSP_ADDR ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define BDR_data  "/BDR.json"
#define size_bdr  2048

// Create an SSD1306 display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // -1 is for the reset pin if not used

const uint8_t bitmap_wifi[] = {0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x06, 0x00, 0x73, 0x00, 0x79, 0x00, 0x0d, 0x80, 0x6d, 0x80, 0x6d, 0x80, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};

//GERAL
String Modo, Perfil, Mac, wifiEr, Conf;
char cModo[7]; char cPerfil[20]; char cMac[6]; char cwifiEr[2]; char cConf[2];

//LOCAL
//portaSupL
String saidaTenL, baudrateL, paridadeL, bitsParaL, contrFluxL, datbitL;
char csaidaTenL[3]; char cbaudrateL[8]; char cparidadeL[10]; char cbitsParaL[2]; char ccontrFluxL[10]; char cdatbitL[2]; 

//wifiL
String ssidL, senwiL;
char cssidL[20]; char csenwiL[20];

//ntripConfL
String hostNtpL, portNtpL, mtpntL, userL, senL;
char chostNtpL[20]; char cportNtpL[6], cmtpntL[80]; char cuserL[40]; char csenL[30];
//LOCAL

//CLIENTE
//portaSupC
String saidaTenC, baudrateC, paridadeC, bitsParaC, contrFluxC, datbitC;
char csaidaTenC[3]; char cbaudrateC[8]; char cparidadeC[10]; char cbitsParaC[2]; char ccontrFluxC[10]; char cdatbitC[2]; 

//wifiC
String ssidC, senwiC;
char cssidC[20]; char csenwiC[20];

//ntripConfL
String hostNtpC, portNtpC, mtpntC, userC, senC;
char chostNtpC[20]; char cportNtpC[20], cmtpntC[80]; char cuserC[40]; char csenC[30];

//latLong
String sendLatLong, lat, lon, alt, pre, utc;
char csendLatLong[2]; char clat[20], clon[20]; char calt[20]; char cpre[20]; char cutc[10];
//CLIENTE

//SOURCE
//portaSupS
String saidaTenS, baudrateS, paridadeS, bitsParaS, contrFluxS, datbitS;
char csaidaTenS[3]; char cbaudrateS[8]; char cparidadeS[10]; char cbitsParaS[2]; char ccontrFluxS[10]; char cdatbitS[2]; 

//wifiS
String ssidS, senwiS;
char cssidS[20]; char csenwiS[20];

//ntripConfS
String hostNtpS, portNtpS, mtpntS, sendRev2, userS, senS;
char chostNtpS[20]; char cportNtpS[20], cmtpntS[80]; char csendRev2[2]; char cuserS[40]; char csenS[30];
//SOURCE

int stateOUT = 1; // Example output state
String SBAUD = "115200";
String ssid;
String password;
String PDvolt = "12V";
const char* mountpoint = "BDR0";
const char* usuario = "HGNP";
const char* senha  = "0102";
uint16_t port = 2101;

int currentPage = 1;  // Start on page 1
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // Debounce time (milliseconds)


unsigned long last_millis;
unsigned long interval = 30000;

const char* hostname = "BDR-30:45";

const char* ssidd = "bisnaguinha5000";
const char* passwordd = "12345678";
int Swifi;
int tentativas;

void wifi_desconectado(WiFiEvent_t event, WiFiEventInfo_t info){
  #if (DEBUG==1)
    Serial.print("event erro: ");Serial.println(event);
    Serial.print("info: ");Serial.printl_desconectadon(info.wifi_sta_disconnected.reason);
  #endif
  if(info.wifi_sta_disconnected.reason==201){
    #if (DEBUG==1)
      Serial.println("Rede não encontrada!");
    #endif
    Swifi = 1;
  }
  if(info.wifi_sta_disconnected.reason==15){
    #if (DEBUG==1)
      Serial.println("senha incorreta!");
    #endif
    Swifi = 2;
  }
  pageWifiStatus();
}

void WiFiEvent(WiFiEvent_t event){
  Serial.print("event: ");Serial.println(event);
  if(event==13){
    #if (DEBUG==1)
      Serial.print(millis()-last_millis);Serial.println(" Millis");
      Serial.println("WiFi Conectado!");
    #endif
    Swifi = 3;
  }
  if(event==16){
    #if (DEBUG==1)
      Serial.print(millis()-last_millis);Serial.println(" Millis");
      Serial.print("IP: ");Serial.println(WiFi.localIP());
    #endif
  }
  pageWifiStatus();
}

void sta_wifi(){
  last_millis = millis();
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(wifi_desconectado, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidd, passwordd);
}

void ap_wifi(){
  last_millis = millis();
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.onEvent(WiFiEvent);
  //WiFi.onEvent(wifi_desconectado, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidd, passwordd);
}

/*bool check_wifi (unsigned long interval, unsigned long retries){
  static int tentativas;
  static unsigned long ultimoMillis;
// if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (millis() - ultimoMillis >=interval)) {
    if(tentativas <= retries - 1){
      ultimoMillis = millis();
      #if (DEBUG==1)
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        Serial.print("tentativas: ");Serial.println(tentativas);
      #endif
      WiFi.disconnect();
      WiFi.reconnect();
    } else {
      WiFi.disconnect(true);
      return 0;
    }
  }else if (WiFi.status() == WL_CONNECTED){
    tentativas = 0;
  }
  return 1;
} */

bool check_wifi() {
    static unsigned long ultimoMillis;
    if (WiFi.status() != WL_CONNECTED) {
      ultimoMillis = millis();
        Serial.print(millis());
        WiFi.disconnect();
        WiFi.reconnect();
    }
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
              wifiEr = strcpy(cwifiEr, avancado["wifiEr"]);
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
    avancado["wifiEr"] = wifiEr.c_str();
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

void fixo() {
    Serial.print("MUITO BEEEM");
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("Modo:");
    display.print(Modo.substring(0, 6));
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
    display.print(Modo.substring(0, 6));
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
    display.print(Modo.substring(0, 6));
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

/*  unsigned long lastTime = 0;
  int scrollPosSSID = 128;  // Initial position for SSID scrolling
  int scrollPosPassword = 128;  // Initial position for Password scrolling

void page1() {
  String ssidText = ("NOMEEEEEEEEEEEEEEEEEE: " + ssid);
  int ssidWidth = ssidText.length() * 6;
  String passwordText = "SENHA: ****************";
  int passwordWidth = passwordText.length() * 6;
  display.fillRect(0, 10, 128, 45, 0);  // Clear the background of the page
  
  // Display "WIFI INFO" at the fixed position
  display.setCursor(35, 12);  // Center the title "WIFI INFO"
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(0);
  display.print("WIFI INFO");

  // Scroll SSID and Password simultaneously
  unsigned long currentMillis = millis();

  if (currentMillis - lastTime > 30) {  // Control scroll speed (every 30 ms)
    // Clear the areas where the SSID and password are being drawn
    display.fillRect(12, 21, 128, 10, 0);  // Clear SSID area
    display.fillRect(12, 33, 128, 10, 0);  // Clear Password area

    // Scroll SSID text
    if (ssidWidth > 128) {
      display.setCursor(scrollPosSSID, 21);  // Update the position for SSID text
      display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
      display.print(ssidText);
      scrollPosSSID -= 2;  // Move the text left by 2 pixels
      if (scrollPosSSID + ssidWidth < 0) {
        scrollPosSSID = 128;  // Reset position when it's off-screen
      }
    } else {
      display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
      display.setCursor(12, 21);
      display.print("NOME:");
      display.print(ssid);  // Just display normally if no scrolling needed
    }

    // Scroll Password text
    if (passwordWidth > 128) {
      display.setCursor(scrollPosPassword, 33);  // Update the position for Password text
      display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
      display.print(passwordText);  // Print the password text
      scrollPosPassword -= 2;  // Move the text left by 2 pixels
      if (scrollPosPassword + passwordWidth < 0) {
        scrollPosPassword = 128;  // Reset position when it's off-screen
      }
    } else {
      display.setCursor(12, 33);
      display.print(passwordText);  // Just display normally if no scrolling needed
    }

    // Display the client count
    display.drawBitmap(0, 42, bitmap_client, 11, 11, 1);
    display.setCursor(12, 44);
    display.print("Clientes:");
    display.print(WiFi.softAPgetStationNum());

    // Update the display
    display.display();

    // Update the last time
    lastTime = currentMillis;
  }
} */

  unsigned long lastTime = 0;
  int scrollPage2Line2 = 128;
  int scrollPage2Line3 = 128;
  int scrollPage2Line4= 128;  

void page2() {
  String line1Text, line2Text, line3Text, line4Text;
  int line2Width, line3Width, line4Width;
  
  if (Modo == "Local") {
    line1Text = ("HOST:" + hostNtpL);
    line2Text = ("PORT:" + portNtpL + "  USER:" + userL);
    line2Width = line2Text.length() * 6;
    line3Text = ("PASSWORD:" + senL);
    line3Width = line3Text.length() * 6;
    line4Text = ("MOUNTPOINT:" + mtpntL);
    line4Width = line4Text.length() * 6;
  } else if (Modo == "Cliente") {
    line1Text = ("HOST:" + hostNtpC);
    line2Text = ("PORT:" + portNtpC + "  USER:" + userC);
    line2Width = line2Text.length() * 6;
    line3Text = ("PASSWORD:" + senC);
    line3Width = line3Text.length() * 6;
    line4Text = ("MOUNTPOINT:" + mtpntC);
    line4Width = line4Text.length() * 6;
  } else if (Modo == "Source") {
    line1Text = ("HOST:" + hostNtpS);
    if (sendRev2 == "S") {
      line2Text = ("PORT:" + portNtpS + "  USER:" + userS);
      line2Width = line2Text.length() * 6;
      line3Text = ("PASSWORD:" + senS);
      line3Width = line3Text.length() * 6;
    } else if (sendRev2 == "N") {
      line2Text = ("PORT:" + portNtpS);
      line2Width = line2Text.length() * 6;
      line3Text = ("USUARIO DESABILITADO");
      line3Width = line3Text.length() * 6;
    }
    line4Text = ("MOUNTPOINT:" + mtpntS);
    line4Width = line4Text.length() * 6;
  }
  display.fillRect(0, 10, 128, 45, 0); 
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(0);
  unsigned long currentMillis = millis();

  display.setCursor(0, 12);
  display.print(line1Text);
  if (currentMillis - lastTime > 30) {
    display.fillRect(0, 22, 128, 10, 0);  
    display.fillRect(0, 32, 128, 10, 0);
    display.fillRect(0, 42, 128, 10, 0);
    if (line2Width > 128) {
      display.setCursor(scrollPage2Line2, 22);
      display.print(line2Text);
      scrollPage2Line2 -= 2;  
      if (scrollPage2Line2 + line2Width < 0) {
        scrollPage2Line2 = 128;  
      }
    } else {
      display.setCursor(0, 22);
      display.print(line2Text);
    }
    if (line3Width > 128) {
      display.setCursor(scrollPage2Line3, 32);
      display.print(line3Text);
      scrollPage2Line3 -= 2;
      if (scrollPage2Line3 + line3Width < 0) {
        scrollPage2Line3 = 128;  
      }
    } else {
      display.setCursor(0, 32);
      display.print(line3Text);
    } if (line4Width > 128) {
      display.setCursor(scrollPage2Line4, 42);
      display.print(line4Text);
      scrollPage2Line4 -= 2;
      if (scrollPage2Line4 + line4Width < 0) {
        scrollPage2Line4 = 128;  
      }
    } else {
      display.setCursor(0, 42);
      display.print(line4Text);
    }
    lastTime = currentMillis;
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

void pageWifi() {
  if (Modo == "Cliente") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(19, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("CONNECTED WIFI");
    display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
    display.setCursor(12, 21);
    display.print("NOME:");
    display.print(ssidC);
    display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
    display.setCursor(12, 33);
    display.print("SENHA:");
    display.print(senwiC);
    display.display();
  } else if (Modo == "Source") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(19, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("CONNECTED WIFI");
    display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
    display.setCursor(12, 21);
    display.print("NOME:");
    display.print(ssidS);
    display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
    display.setCursor(12, 33);
    display.print("SENHA:");
    display.print(senwiS);
    display.display();
  }
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
  display.setCursor(70, 22);
  display.print("Long:");
  display.print(lon.substring(0, 5));
  display.setCursor(0, 32);
  display.print("Alti:");
  display.print(alt.substring(0, 5));
  display.setCursor(70, 32);
  display.print("Prec:");
  display.print(pre.substring(0, 5));
  display.setCursor(0, 42);
  display.print("UTC Time:");
  display.print(utc);
  display.display();
}

void pageWifiStatus() {
  String line1Text, line2Text, line3Text, line4Text;
  int line2Width, line3Width, line4Width;
  
  if (Swifi == 1) {
    line1Text = ("WIFI STATUS");
    line2Text = ("ERRO:1");
    line2Width = line2Text.length() * 6;
    line3Text = ("REDE NAO ENCONTRADA");
    line3Width = line3Text.length() * 6;
  } else if (Swifi == 2) {
    line1Text = ("WIFI STATUS");
    line2Text = ("ERRO:2");
    line2Width = line2Text.length() * 6;
    line3Text = ("SENHA INCORRETA");
    line3Width = line3Text.length() * 6;
  } else if (Swifi == 3) {
    line1Text = ("WIFI STATUS");
    line2Text = ("CONECTADO");
    line2Width = line2Text.length() * 6;
    if (Modo == "Cliente") {
     line3Text = ("REDE:" + ssidC); 
    } else if (Modo == "Source") {
      line3Text = ("REDE:" + ssidS);
    }
    line3Width = line3Text.length() * 6;
  }
  display.fillRect(0, 10, 128, 45, 0); 
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(0);
  unsigned long currentMillis = millis();

  display.setCursor(30, 12);
  display.print(line1Text);
  if (currentMillis - lastTime > 30) {
    display.fillRect(0, 22, 128, 10, 0);  
    display.fillRect(0, 32, 128, 10, 0);
    display.fillRect(0, 42, 128, 10, 0);
    if (line2Width > 128) {
      display.setCursor(scrollPage2Line2, 22);
      display.print(line2Text);
      scrollPage2Line2 -= 2;  
      if (scrollPage2Line2 + line2Width < 0) {
        scrollPage2Line2 = 128;  
      }
    } else {
      display.setCursor(0, 22);
      display.print(line2Text);
    }
    if (line3Width > 128) {
      display.setCursor(scrollPage2Line3, 32);
      display.print(line3Text);
      scrollPage2Line3 -= 2;
      if (scrollPage2Line3 + line3Width < 0) {
        scrollPage2Line3 = 128;  
      }
    } else {
      display.setCursor(0, 32);
      display.print(line3Text);
    }
    lastTime = currentMillis;
    display.display();
  }
}

/*void page_pd(){
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
} */

void setup() {
  pins_init();
  loaddata(BDR_data);
  digitalWrite(DPWR,HIGH);
  digitalWrite(LED1B,HIGH);
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  Serial1.begin(SBAUD.toInt(), SERIAL_8N1, RX, TX);
  WiFi.disconnect(true);
  delay(1000);
  Serial.println("teste de wifi");
  sta_wifi();

  //WiFi.softAP("BDR-", NULL, 7, 0, 5);
  //String macAddress = WiFi.softAPmacAddress();
  //String lastFourDigits = macAddress.substring(macAddress.length() - 5);
  //macAddress.replace(":", "");
  //password = macAddress.substring(macAddress.length() - 8);
  //ssid = "BDR-" + lastFourDigits;
  //WiFi.softAP(ssid, password);

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the display
  display.clearDisplay();
  display.display();
  // Call your function
  fixo();
  page1();
}

void loop() {
  // Read the button state
  if (digitalRead(PAGE) == HIGH) {
    // Button was pressed
    if (millis() - lastButtonPress > debounceDelay) {
       currentPage = (currentPage % 7) + 1;  // Toggle between page 1, 2, and 3
      lastButtonPress = millis();  // Update the time of the last button press
    }
  }

  // Display the appropriate page
 /* if (currentPage == 1) {
    page1();
  } else if (currentPage == 2) {
    page2();
  } else if (currentPage == 3) {
    page3();
  } else if (currentPage == 4) {
    page4();
  } else if (currentPage == 5) {
    pageLoc();
  } else if (currentPage == 6) {
    pageWifi();
  } else if (currentPage == 7) {
    pageWifiStatus();
  } */

  if (Swifi == 1 || Swifi == 2){
  check_wifi();
  }
}
