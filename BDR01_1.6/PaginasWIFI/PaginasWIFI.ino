#include "WiFi.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "ESPAsyncWebServer.h"
#include "DNSServer.h"
#include "AsyncTCP.h"

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
const uint8_t wifi_icon [] = {0xe0, 0x1f, 0x80, 0x03, 0x80, 0x01, 0x00, 0x01, 0xff, 0x01, 0x81, 0x80, 0x00, 0xc0, 0x00, 0x60, 0xf8, 0x30, 0x8c, 0x10, 0x06, 0x10, 0x03, 0x10, 0xe1, 0x10, 0x31, 0x11, 0x11, 0x11, 0x13, 0x37};
const unsigned char wifi_icon2 [] = {0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0xEC, 0xEE, 0xCE, 0x1E, 0xFC, 0xF9, 0x73, 0x07, 0xFF, 0xFF, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char wifi_icon3 [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0xEC, 0xEE, 0xCE, 0x1E, 0xFC, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};

//GERAL
String Modo, Perfil, Mac, wifiEr, Conf, safeM;
char cModo[7]; char cPerfil[20]; char cMac[6]; char cwifiEr[2]; char cConf[2]; char csafeM[2];

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

AsyncWebServer server(80);

IPAddress IP = WiFi.localIP();
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
const unsigned long debounceDelay = 500;


unsigned long last_millis;
unsigned long interval = 30000;

const char* hostname = "BDR-30:45";

const char* ssidd = "bisnaguinha5000";
const char* passwordd = "12345678";
int Swifi = 0;
int lastSwifi = 1;
int tentativas;

void wifi_desconectado(WiFiEvent_t event, WiFiEventInfo_t info){
  #if (DEBUG==1)
    Serial.print("event erro: ");Serial.println(event);
    Serial.print("info: ");Serial.println(info.wifi_sta_disconnected.reason);
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
  if (Swifi != lastSwifi) {
    Serial.println("Wifi Error Page");
  }
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
      IP = WiFi.localIP();
      Serial.print("IP: ");Serial.println(IP);
    #endif
  }
  if (Swifi != lastSwifi) {
    Serial.println("Wifi Error Page");
  }
}

void sta_wifi(){
  last_millis = millis();
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(wifi_desconectado, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidd, passwordd);
  if (Modo == "Cliente") {
    WiFi.disconnect();
    WiFi.begin(ssidC, senwiC);
    IP = WiFi.localIP();
  } else if (Modo == "Source") {
    WiFi.disconnect();
    WiFi.begin(ssidS, senwiS);
    IP = WiFi.localIP();
  }
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

bool check_wifi (unsigned long interval, unsigned long retries){
  static int tentativas;
  static unsigned long ultimoMillis;
  int lastStatus = WL_DISCONNECTED; // Store the last Wi-Fi status
  int currentStatus = WiFi.status(); // Get the current Wi-Fi status
// if WiFi is down, try reconnecting
    if ((currentStatus != WL_CONNECTED) && (millis() - ultimoMillis >=interval)) {
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
}

/*bool check_wifi() {
    static unsigned long ultimoMillis;
    int lastStatus = WL_DISCONNECTED; // Store the last Wi-Fi status
    int currentStatus = WiFi.status(); // Get the current Wi-Fi status
    if (currentStatus != WL_CONNECTED) {
      ultimoMillis = millis();
        Serial.print(millis());
        WiFi.disconnect();
        WiFi.reconnect();
    }

     Serial.println(lastStatus);
     Serial.println(currentStatus);
     if (currentStatus != lastStatus) {
        // Wi-Fi status has changed, perform actions based on the new status
        if (currentStatus == WL_CONNECTED) {
            Serial.println("Wi-Fi connected");
            // Additional commands when Wi-Fi is connected
        } else if (currentStatus == WL_DISCONNECTED) {
            Serial.println("Wi-Fi disconnected");
            // Additional commands when Wi-Fi is disconnected
        }
        // Update the lastStatus to the current status
        lastStatus = currentStatus;
    }
} */

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
              safeM = strcpy(csafeM, avancado ["safeM"]);
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
    avancado["safeM"] = safeM.c_str();
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
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    if (Conf == "1") {
      display.print("Modo:");
      display.print("Config");
    } else {
      display.print("Modo:");
      display.print(Modo.substring(0, 6));
    }
    display.setCursor(74, 0);
    display.print("Saida  :");
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

void fixo2() {
    display.fillRect(0, 0, 128, 8, 0);
    display.fillRect(0, 56, 128, 64, 0);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    if (Conf == "1") {
      display.print("Modo:");
      display.print("Config");
    } else {
      display.print("Modo:");
      display.print(Modo.substring(0, 6));
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
    display.print("EXIT");
    display.display();
}

/*void fixoConfirm() {
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

/*
void pageConf(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(35, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("WIFI INFO");
    display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
    display.setCursor(12, 21);
    display.print("NOME:");
    display.print(ssid);
    display.drawBitmap(0, 31, bitmap_client, 10, 10, 1);
    display.setCursor(12, 33);
    display.print("Clientes:");
    display.print(WiFi.softAPgetStationNum());
    display.display();
} */

void pageConf(){
    if (WiFi.softAPgetStationNum() >= 1) {
      display.fillRect(0, 10, 128, 45, 0);
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(35, 34);
      display.print("CONECTADO");
      display.setCursor(15, 45);
      display.print("VERIFIQUE O APP");
      display.display();
    } else {
      display.fillRect(0, 10, 128, 45, 0);
      display.setCursor(4, 12);
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.print("MODO DE CONFIGURACAO");
      display.drawBitmap(23, 43, bitmap_wifi, 10, 10, 1);
      display.setCursor(15, 33);
      display.print("CONECTAR AO WIFI");
      display.setCursor(35, 44);
      display.print(ssid);
      display.display();
    }
}

/*  unsigned long lastTime = 0;
  int scrollPosSSID = 128;  // Initial position for SSID scrolling
  int scrollPosPassword = 128;  // Initial position for Password scrolling

void pageConf() {
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
 /*
  unsigned long lastTime = 0;
  int scrollPage2Line2 = 128;
  int scrollPage2Line3 = 128;
  int scrollPage2Line4= 128;  

  void page1() {
  String line1Text, line2Text, line3Text, line4Text;
  int line2Width, line3Width, line4Width;
  
  if (Modo == "Local") {
    line1Text = ("WIFI INFO");
    line2Text = ("SSID:" + ssidL);
    line2Width = line2Text.length() * 6;
    line3Text = ("PASS:" + senwiL);
    line3Width = line3Text.length() * 6;
    line4Text = ("USERS:" + String(WiFi.softAPgetStationNum()));
    line4Width = line4Text.length() * 6;
    //line1Text = ("HOST:" + hostNtpL);
    //line2Text = ("PORT:" + portNtpL + "  USER:" + userL);
    //line2Width = line2Text.length() * 6;
    //line3Text = ("PASSWORD:" + senL);
    //line3Width = line3Text.length() * 6;
    //line4Text = ("MOUNTPOINT:" + mtpntL);
    //line4Width = line4Text.length() * 6;
  } else if (Modo == "Cliente") {
    line1Text = ("STATUS BDR");
    line2Text = ("WIFI:-----");
    line2Width = line2Text.length() * 6;
    line3Text = ("NTRIP:-----");
    line3Width = line3Text.length() * 6;
    line4Text = ("SAIDA:-----");
    line4Width = line4Text.length() * 6;
    //line1Text = ("HOST:" + hostNtpC);
    //line2Text = ("PORT:" + portNtpC + "  USER:" + userC);
    //line2Width = line2Text.length() * 6;
    //line3Text = ("PASSWORD:" + senC);
    //line3Width = line3Text.length() * 6;
    //line4Text = ("MOUNTPOINT:" + mtpntC);
    //line4Width = line4Text.length() * 6;
  } else if (Modo == "Source") {
    line1Text = ("STATUS BDR");
    line2Text = ("WIFI:-----");
    line2Width = line2Text.length() * 6;
    line3Text = ("NTRIP::-----");
    line3Width = line3Text.length() * 6;
    line4Text = ("SAIDA:-----");
    line4Width = line4Text.length() * 6;
  }
  display.fillRect(0, 10, 128, 45, 0); 
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(0);
  unsigned long currentMillis = millis();

  display.setCursor(35, 12);
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
} */
void page1() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  if (Modo == "Local"){
    display.setCursor(35, 12);
    display.print("WIFI INFO");
    display.setCursor(0, 22);
    display.print("SSID:" + ssidL);
    display.setCursor(0, 32);
    display.print("PASS:" + senwiL);
    display.setCursor(0, 42);
    display.print("USERS:" + String(WiFi.softAPgetStationNum()));
    display.display();
  } else if (Modo == "Cliente") {
    display.setCursor(30, 12);
    display.print("STATUS BDR");
    display.setCursor(0, 22);
    display.print("WIFI:");
    display.setCursor(32,22);
    if (Swifi == 0) {
      display.print("...");
    } else if (Swifi == 1) {
      display.print("NAO ENCONTRADO");
    } else if (Swifi == 2) {
      display.print("SENHA INVALIDA");
    } else if (Swifi == 3) {
      display.print("CONECTADO");
    }
    display.setCursor(0, 32);
    display.print("NTRIP:-----");
    display.setCursor(0, 42);
    display.print("SAIDA:-----");
    display.display();
  } else if (Modo == "Source") {
    display.setCursor(30, 12);
    display.print("STATUS BDR");
    display.setCursor(0, 22);
    display.print("WIFI:");
    display.setCursor(32,22);
    if (Swifi == 0) {
      display.print("...");
    } else if (Swifi == 1) {
      display.print("NAO ENCONTRADO");
    } else if (Swifi == 2) {
      display.print("SENHA INVALIDA");
    } else if (Swifi == 3) {
      display.print("CONECTADO");
    }
    display.setCursor(0, 32);
    display.print("NTRIP:-----");
    display.setCursor(0, 42);
    display.print("SAIDA:-----");
    display.display();
  }
}

unsigned long lastTime = 0;
bool scrollCheck = 0;
int scrollPage2Line2, scrollPage2Line4, scrollPage2Line2Info2, scrollPage2Line3, scrollPage2Line4Info2;
bool LoRP2L4, LoRP2L20, LoRP2L2I1, LoRP2L2I2, LoRP2L3, LoRP2L4I1, LoRP2L4I2, LoRP2L2;

void page2() {
  String line1Text, line2Text, line2Text2, line3Text, line4Text, line4Text2;
  int line2Width, line2T2Width, line3Width, line4Width, line4T2Width;

  if (scrollCheck == 0) {
    Serial.println("scrollcheck");
    if (Modo == "Cliente" || Modo == "Source") {
      Serial.println("scrollcheckclisou");
      scrollPage2Line2 = 30;
      scrollPage2Line4 = 18;
      LoRP2L4 = 0;
      LoRP2L2 = 0;
    } else if (Modo == "Local") {
      Serial.println("scrollchecklocal");
      scrollPage2Line2 = 30;
      scrollPage2Line2Info2 = 65;
      scrollPage2Line3 = 30;
      scrollPage2Line4 = 30;
      scrollPage2Line4Info2 = 65;
      LoRP2L2I1 = 0;
      LoRP2L2I2 = 0;
      LoRP2L3 = 0;
      LoRP2L4I1 = 0;
      LoRP2L4I2 = 0;
    }
    scrollCheck = 1;
  }

  if (Modo == "Local") {
    line1Text = ("NTRIP INFO");
    line2Text = (hostNtpL);
    line2Width = line2Text.length() * 6;
    line2Text2 = (portNtpL);
    line2T2Width = line2Text2.length() * 6;
    line3Text = (mtpntL);
    line3Width = line3Text.length() * 6;
    line4Text = (userL);
    line4Width = line4Text.length() * 6;
    line4Text2 = (senL);
    line4T2Width = line4Text2.length() * 6;
  } else if (Modo == "Cliente") {
    line1Text = ("WIFI STATUS");
    line2Text = (ssidC);
    line2Width = line2Text.length() * 6;
    if (Swifi == 0) {
      line3Text = ("STATUS:...");
    } else if (Swifi == 1) {
      line3Text = ("STATUS:NAO ENCONTRADO");
    } else if (Swifi == 2) {
      line3Text = ("STATUS:SENHA INCORRETA");
    } else if (Swifi == 3) {
      line3Text = ("STATUS:CONECTADO");
    }
    line3Width = line3Text.length() * 6;
    String IPStr = IP.toString();
    line4Text = (IPStr + " RSSI:" + String(WiFi.RSSI()));
    line4Width = line4Text.length() * 6;
  } else if (Modo == "Source") {
    line1Text = ("WIFI STATUS");
    line2Text = (ssidS);
    line2Width = line2Text.length() * 6;
    if (Swifi == 0) {
      line3Text = ("STATUS:...");
    } else if (Swifi == 1) {
      line3Text = ("STATUS:NAO ENCONTRADO");
    } else if (Swifi == 2) {
      line3Text = ("STATUS:SENHA INCORRETA");
    } else if (Swifi == 3) {
      line3Text = ("STATUS:CONECTADO");
    }
    line3Width = line3Text.length() * 6;
    String IPStr = IP.toString();
    line4Text = (IPStr);
    line4Width = line4Text.length() * 6;
  }

  if (Modo == "Cliente" || Modo == "Source") {
    display.fillRect(0, 12, 128, 10, 0); 
    display.fillRect(0, 32, 128, 10, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(0);
    unsigned long currentMillis = millis();
    display.setCursor(28, 12);
    display.print(line1Text);
    display.setCursor(0, 32);
    display.print(line3Text);
    
    if (currentMillis - lastTime > 100) {
      display.fillRect(0, 22, 128, 10, 0);
      display.fillRect(0, 42, 128, 10, 0);
      if (line2Width > 98) {
        display.setCursor(scrollPage2Line2, 22);
        display.print(line2Text);
        display.fillRect(0, 22, 30, 10, 0);
        display.setCursor(0, 22);
        display.print("REDE:");
        if (LoRP2L2 == 0) {
            if (scrollPage2Line2 + (line2Width - 126) != 0) {
              scrollPage2Line2 -= 1;
              if (scrollPage2Line2 + (line2Width - 126) == 0) {
                  LoRP2L2 = 1;
                }
              }
          }
          if (LoRP2L2 == 1) {
            if (scrollPage2Line2 != 32) {
              scrollPage2Line2 += 1;
              if (scrollPage2Line2 == 32) {
                  LoRP2L2 = 0;
              }
            }
          }
      } else {
        display.setCursor(30, 22);
        display.print(line2Text);
        display.fillRect(0, 22, 30, 10, 0);
        display.setCursor(0, 22);
        display.print("REDE:");
      }
       if (line4Width >= 61) {
        display.setCursor(scrollPage2Line4, 42);
        display.print(line4Text);
        display.fillRect(0, 42, 18, 10, 0);
        display.setCursor(0, 42);
        display.print("IP:");
        display.fillRect(79, 42, 49, 10, 0);
        display.setCursor(80, 42);
        display.print("RSSI:" + String(WiFi.RSSI()));
        if (LoRP2L4 == 0) {
          if (scrollPage2Line4 + (line4Width - 77) != 0) {
            scrollPage2Line4 -= 1;
            if (scrollPage2Line4 + (line4Width - 77) == 0) {
                LoRP2L4 = 1;
              }
            }
          }
          if (LoRP2L4 == 1) {
            if (scrollPage2Line4 != 20) {
              scrollPage2Line4 += 1;
              if (scrollPage2Line4 == 20) {
                  LoRP2L4 = 0;
              }
            }
          }
      } else {
          display.setCursor(18, 42);
          display.print(line4Text);
          display.fillRect(0, 42, 18, 10, 0);
          display.setCursor(0, 42);
          display.print("IP:");
          display.fillRect(79, 42, 49, 10, 0);
          display.setCursor(80, 42);
          display.print("RSSI:" + String(WiFi.RSSI()));
      }
      lastTime = currentMillis;
      display.display();
    }
  } else if (Modo == "Local") {
    display.fillRect(0, 12, 128, 10, 0); 
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(0);
    unsigned long currentMillis = millis();
    display.setCursor(28, 12);
    display.print(line1Text);
    
    if (currentMillis - lastTime > 100) {
      display.fillRect(0, 22, 128, 10, 0);
      display.fillRect(0, 32, 128, 10, 0);
      display.fillRect(0, 42, 128, 10, 0);
      
      if (line2Width > 35) {
        display.setCursor(scrollPage2Line2, 22);
        display.print(line2Text);
        display.fillRect(0, 22, 30, 10, 0);
        display.setCursor(0, 22);
        display.print("HOST:");
        if (LoRP2L2I1 == 0) {
            if ((scrollPage2Line2 - 30) + (line2Width - 33) != 0) {
              scrollPage2Line2 -= 1;
              Serial.println(line2Width);
              Serial.println(scrollPage2Line2);
              if ((scrollPage2Line2 - 30) + (line2Width - 33) == 0) {
                  LoRP2L2I1 = 1;
                }
              }
          }
          if (LoRP2L2I1 == 1) {
            if (scrollPage2Line2 != 32) {
              scrollPage2Line2 += 1;
              Serial.println(line2Width);
              Serial.println(scrollPage2Line2);
              if (scrollPage2Line2 == 32) {
                  LoRP2L2I1 = 0;
              }
            }
          }
      } else {
        display.setCursor(30, 22);
        display.print(line2Text);
        display.fillRect(0, 22, 30, 10, 0);
        display.setCursor(0, 22);
        display.print("HOST:");
      }

      if (line2T2Width > 33) {
        display.setCursor(scrollPage2Line2Info2, 22);
        display.print(line2Text2);
        display.fillRect(65, 22, 30, 10, 0);
        display.setCursor(0, 22);
        display.print("PORT:");
        if (LoRP2L2I2 == 0) {
            if ((scrollPage2Line2Info2 - 65) + (line2T2Width - 31) != 0) {
              scrollPage2Line2Info2 -= 1;
              if ((scrollPage2Line2Info2 - 65) + (line2T2Width - 31) == 0) {
                  LoRP2L2I2 = 1;
                }
              }
        }
        if (LoRP2L2I2 == 1) {
          if (scrollPage2Line2Info2 != 67) {
            scrollPage2Line2Info2 += 1;
            if (scrollPage2Line2Info2 == 67) {
                LoRP2L2I2 = 0;
            }
          }
        }
      } else {
        display.fillRect(65, 22, 63, 10, 0);
        display.setCursor(95, 22);
        display.print(line2Text2);
        display.setCursor(65, 22);
        display.print("PORT:");
      }

      if (line3Width > 98) {
        display.setCursor(scrollPage2Line3, 32);
        display.print(line3Text);
        display.fillRect(0, 32, 30, 10, 0);
        display.setCursor(0, 32);
        display.print("MNTP:");
        if (LoRP2L3 == 0) {
            if ((scrollPage2Line3 - 30) + (line3Width - 96) != 0) {
              scrollPage2Line3 -= 1;
              Serial.println(line3Width);
              Serial.println(scrollPage2Line3);
              if ((scrollPage2Line3 - 30) + (line3Width - 96) == 0) {
                  LoRP2L3 = 1;
                }
              }
          }
          if (LoRP2L3 == 1) {
            if (scrollPage2Line3 != 32) {
              scrollPage2Line3 += 1;
              Serial.println(line3Width);
              Serial.println(scrollPage2Line3);
              if (scrollPage2Line3 == 32) {
                  LoRP2L3 = 0;
              }
            }
          }
      } else {
        display.setCursor(30, 32);
        display.print(line3Text);
        display.fillRect(0, 32, 30, 10, 0);
        display.setCursor(0, 32);
        display.print("MNTP:");
      }
      
      
      lastTime = currentMillis;
      display.display();
    }
  }
}

int scrollPage3Line2 = 128;
int scrollPage3Line3 = 128;
int scrollPage3Line4= 128;  

void page3() {
  String line1Text, line2Text, line3Text, line4Text;
  int line2Width, line3Width, line4Width;

  if (Modo == "Local") {
    line1Text = ("SERIAL PORT");
    line2Text = ("BAUD:" + baudrateL);
    line2Width = line2Text.length() * 6;
    line3Text = ("PARI:" + paridadeL + " DATA:" + datbitL);
    line3Width = line3Text.length() * 6;
    line4Text = ("STOP:" + bitsParaL + " FLOW:" + contrFluxL);
    line4Width = line4Text.length() * 6;
  } else if (Modo == "Cliente") {
    line1Text = ("NTRIP STATUS");
    line2Text = ("REDE:" + hostNtpC);
    line2Width = line2Text.length() * 6;
    line3Text = ("LATENCIA:");
    line3Width = line3Text.length() * 6;
    line4Text = ("UPTIME:");
    line4Width = line4Text.length() * 6;
  } else if (Modo == "Source") {
    line1Text = ("NTRIP STATUS");
    line2Text = ("REDE:" + hostNtpS);
    line2Width = line2Text.length() * 6;
    line3Text = ("LATENCIA:");
    line3Width = line3Text.length() * 6;
    line4Text = ("UPTIME:");
    line4Width = line4Text.length() * 6;
  }
  
  display.fillRect(0, 10, 128, 45, 0); 
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(0);
  unsigned long currentMillis = millis();
  display.setCursor(26, 12);
  display.print(line1Text);
  
  if (currentMillis - lastTime > 30) {
    display.fillRect(0, 22, 128, 10, 0);  
    display.fillRect(0, 32, 128, 10, 0);
    display.fillRect(0, 42, 128, 10, 0);
    if (line2Width > 128) {
      display.setCursor(scrollPage3Line2, 22);
      display.print(line2Text);
      scrollPage3Line2 -= 2;  
      if (scrollPage3Line2 + line2Width < 0) {
        scrollPage3Line2 = 128;  
      }
    } else {
      display.setCursor(0, 22);
      display.print(line2Text);
    }
    if (line3Width > 128) {
      display.setCursor(scrollPage3Line3, 32);
      display.print(line3Text);
      scrollPage3Line3 -= 2;
      if (scrollPage3Line3 + line3Width < 0) {
        scrollPage3Line3 = 128;  
      }
    } else {
      display.setCursor(0, 32);
      display.print(line3Text);
    } if (line4Width > 128) {
      display.setCursor(scrollPage3Line4, 42);
      display.print(line4Text);
      scrollPage3Line4 -= 2;
      if (scrollPage3Line4 + line4Width < 0) {
        scrollPage3Line4 = 128;  
      }
    } else {
      display.setCursor(0, 42);
      display.print(line4Text);
    }
    lastTime = currentMillis;
    display.display();
  }
}

void page4() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  if (Modo == "Local"){
    display.setCursor(25, 12);
    display.print("SAIDA STATUS");
    display.setCursor(0, 22);
    display.print("STATUS:--- TENSAO:---");
    display.setCursor(0, 32);
    display.print("PACOTES:---");
    display.setCursor(0, 42);
    display.print("LATENCIA:---");
    display.display();
  } else if (Modo == "Cliente") {
    display.setCursor(25, 12);
    display.print("SAIDA STATUS");
    display.setCursor(0, 22);
    display.print("STATUS:--- TENSAO:---");
    display.setCursor(0, 32);
    display.print("PACOTES:---");
    display.setCursor(0, 42);
    display.print("LATENCIA:---");
    display.display();
  } else if (Modo == "Source") {
    display.setCursor(25, 12);
    display.print("SAIDA STATUS");
    display.setCursor(0, 22);
    display.print("STATUS:--- TENSAO:---");
    display.setCursor(0, 32);
    display.print("PACOTES:---");
    display.setCursor(0, 42);
    display.print("LATENCIA:---");
    display.display();
  }
}

void page5() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(15, 12);
  display.print("INFO ALIMENTACAO");
  display.setCursor(0, 22);
  display.print("STATUS:---");
  display.setCursor(0, 32);
  display.print("TENSOES:");
  display.setCursor(0, 42);
  display.print("5V, 9V, 12V, 15V, 20V");
}

void page6() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(35, 12);
  display.print("BDR INFO");
  display.setCursor(0, 22);
  display.print("HUGEN PLUS");
  display.setCursor(0, 32);
  display.print("SERIAL:-----");
  display.setCursor(0, 42);
  display.print("MODEL:-----  FIRM:-----");
}

/*void page4confirm(){
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
  if (Modo == "Local") {
    display.print(saidaTenL);
  }
  else if (Modo == "Cliente") {
    display.print(saidaTenC);
  }
  else if (Modo == "Source") {
    display.print(saidaTenS);
  }
  display.display();
} */

void pageExit(){
  if (Conf == "1") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(10, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("CONFIGURATION MODE");
    display.setCursor(50, 33);
    display.print("EXIT");
    display.setCursor(10, 44);
    display.print("CONFIGURATION MODE?");
    display.display();
  } else if (Conf == "0") {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(10, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("CONFIGURATION MODE");
    display.setCursor(50, 33);
    display.print("ENTER");
    display.setCursor(10, 44);
    display.print("CONFIGURATION MODE?");
    display.display();
  }
}

/*void pageWifi() {
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
    display.setCursor(0, 43);
    display.print("RSSI:" + String(WiFi.RSSI()));
    display.drawBitmap(25, 43, wifi_icon, 10, 10, 1);
    if (WiFi.RSSI() >= -70) {
      display.drawBitmap(25, 43, wifi_icon, 10, 10, 1);
    } else if (WiFi.RSSI() < -70 && WiFi.RSSI() > -100) {
      display.drawBitmap(25, 43, wifi_icon2, 10, 10, 1);
    } else if (WiFi.RSSI() <= -100) {
      display.drawBitmap(25, 43, wifi_icon3, 10, 10, 1);
    }
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
    display.setCursor(0, 43);
    display.print("RSSI:" + String(WiFi.RSSI()));
    //display.drawBitmap(25, 43, wifi_icon, 10, 10, 1);
    /*if (WiFi.RSSI() >= -70) {
      display.drawBitmap(25, 43, wifi_icon, 10, 10, 1);
    } else if (WiFi.RSSI() < -70 && WiFi.RSSI() > -100) {
      display.drawBitmap(25, 43, wifi_icon2, 10, 10, 1);
    } else if (WiFi.RSSI() <= -100) {
      display.drawBitmap(25, 43, wifi_icon3, 10, 10, 1);
    }
    display.display();
  }
} */

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

/*void pageWifiStatus() {
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
} */

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
  Serial.println(saidaTenL);
  Serial.println(baudrateL);
  Serial.println(paridadeL);
  Serial.println(bitsParaL);
  Serial.println(contrFluxL);
  Serial.println(datbitL);
  Serial.println(ssidL);
  Serial.println(senwiL);
  Serial.println(hostNtpL);
  Serial.println(portNtpL);
  Serial.println(mtpntL);
  Serial.println(userL);
  Serial.println(senL);
  Serial.println(Modo);
  Serial.println(Perfil);
  Serial1.begin(SBAUD.toInt(), SERIAL_8N1, RX, TX);
  WiFi.disconnect(true);
  delay(1000);
  Serial.println("teste de wifi");
  if(Conf == "1") {
    configMode();
  } else if (Conf == "0" && Modo == "Local") {
    local();
  } else if (Conf == "0" && Modo == "Cliente") {
    cliente();
  } else if (Conf == "0" && Modo == "Source") {
    source();
  }
  
  /*WiFi.softAP("BDR-", NULL, 7, 0, 5);
  String macAddress = WiFi.softAPmacAddress();
  String lastFourDigits = macAddress.substring(macAddress.length() - 5);
  macAddress.replace(":", "");
  password = macAddress.substring(macAddress.length() - 8);
  ssid = "BDR-" + lastFourDigits;
  WiFi.softAP(ssid, password); */

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
  pageConf();
}

void configMode() {
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  WiFi.softAP("BDR-", NULL, 7, 0, 5);
  String macAddress = WiFi.softAPmacAddress();
  String lastFourDigits = macAddress.substring(macAddress.length() - 5);
  macAddress.replace(":", "");
  //password = macAddress.substring(macAddress.length() - 8);
  ssid = "BDR-" + lastFourDigits;
  WiFi.softAP(ssid.c_str(), NULL, 5, 0, 5);
  Mac = lastFourDigits;
  saveConfigFile(BDR_data);

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
            saveConfigFile(BDR_data);
            request->send(200);
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
        server.on("/erroCorrec", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
        if (request->hasParam("wifiEr", true)) {
            AsyncWebParameter* da = request->getParam("wifiEr", true);
            wifiEr = da->value();
        }
            saveConfigFile(BDR_data);
            request->send(200);
        });
        server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Received POST request");
            Conf = "0";
            saveConfigFile(BDR_data);
            request->send(200);
            delay(100);
            ESP.restart();
        });

      saveConfigFile(BDR_data);
      server.begin();
}

void local() {
  
}

void cliente() {
  sta_wifi();
}

void source() {
  sta_wifi();
}

String baudRates[] = {"9600", "19200", "38400", "57600", "115200"};
int currentBaudIndex = 0;
String powerOut[] = {"5V", "9V", "12V", "15V", "20V"};
int currentPowerIndex = 0;

void pageChange() {
  if (Conf == "1") {
    if (digitalRead(PAGE) == HIGH) {
        if (millis() - lastButtonPress > debounceDelay) {
          currentPage = (currentPage % 2) + 1;
          lastButtonPress = millis();
      }
    }
  
    if (currentPage == 1) {
      pageConf();
      fixo();
    } else if (currentPage == 2) {
      pageExit();
      fixo2();
      if (digitalRead(ENTER) == HIGH) {
        Conf = "0";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
  }
  
    else if (Modo == "Local" && Conf == "0") {
      if (digitalRead(PAGE) == HIGH) {
        if (millis() - lastButtonPress > debounceDelay) {
          currentPage = (currentPage % 7) + 1;
          lastButtonPress = millis();
      }
    }
  
    if (currentPage == 1) {
      page1();
      fixo();
    } else if (currentPage == 2) {
      page2();
      fixo();
    }else if (currentPage == 3) {
      page3();
      fixo();
    } else if (currentPage == 4) {
      page4();
      fixo();
    } else if (currentPage == 5) {
      page5();
      fixo();
    } else if (currentPage == 6) {
      page6();
      fixo();
    } else if (currentPage == 7) {
      pageExit();
      fixo2();
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    check_wifi(1000, 3);
    
  } else if (Modo == "Cliente" && Conf == "0") {
    if (digitalRead(PAGE) == HIGH) {
        if (millis() - lastButtonPress > debounceDelay) {
          currentPage = (currentPage % 8) + 1;
          lastButtonPress = millis();
      }
    }
  
    if (currentPage == 1) {
      page1();
      fixo();
    } else if (currentPage == 2) {
      page2();
      fixo();
    } else if (currentPage == 3) {
      page3();
      fixo();
    } else if (currentPage == 4) {
      page4();
      fixo();
    } else if (currentPage == 5) {
      page5();
      fixo();
    } else if (currentPage == 6) {
      page6();
      fixo();
    } else if (currentPage == 7) {
      pageLoc();
      fixo();
    } else if (currentPage == 8) {
      pageExit();
      fixo2();
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    check_wifi(1000, 3);
    
  } else if (Modo == "Source" && Conf == "0") {
    if (digitalRead(PAGE) == HIGH) {
        if (millis() - lastButtonPress > debounceDelay) {
          currentPage = (currentPage % 7) + 1;
          lastButtonPress = millis();
      }
    }
  
    if (currentPage == 1) {
      page1();
      fixo();
    } else if (currentPage == 2) {
      page2();
      fixo();
    }else if (currentPage == 3) {
      page3();
      fixo();
    } else if (currentPage == 4) {
      page4();
      fixo();
    } else if (currentPage == 5) {
      page5();
      fixo();
    } else if (currentPage == 6) {
      page6();
      fixo();
    } else if (currentPage == 7) {
      pageExit();
      fixo2();
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    check_wifi(1000, 3);
  }
}

void loop() {
  pageChange();
}
