#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Scroller.h"
#include "BDR01_display.h"
#include "WiFi.h"
#include "SPI.h"
#include "Wire.h"
#include "HardwareSerial.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "NTRIPServer.h"
#include "NTRIPSource.h"
#include "NTRIPClient.h"

#define BDR_data  "/BDR.json"
#define size_bdr  2048

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
#define EXTINV 37
#define EXTOFF 36
#define EXTEN 35
#define LED1B 43
#define LED1R 44
#define LED1G 42
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DSP_ADDR 0x3c
#define OLED_RESET     DRST
#define SCREEN_ADDRESS DSP_ADDR

BDRDisplay displaybdr;

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
  pinMode(EXTEN, OUTPUT);
  pinMode(EXTOFF, OUTPUT);
  pinMode(EXTINV, INPUT);
}

//GERAL
String Modo, Perfil, Mac, wifiEr, Conf, voltConf;
char cModo[7]; char cPerfil[20]; char cMac[6]; char cwifiEr[2]; char cConf[2]; char cvoltConf[4];

int portConnected = 0;

//LOCAL
//portaSupL
String saidaTenL, baudrateL, paridadeL, bitsParaL, contrFluxL, datbitL;
char csaidaTenL[3]; char cbaudrateL[8]; char cparidadeL[10]; char cbitsParaL[2]; char ccontrFluxL[10]; char cdatbitL[2]; 

//wifiL
String ssidL, senwiL;
char cssidL[20]; char csenwiL[20];

//ntripConfL
String hostNtpL, portNtpL, mtpntL, userL, senL;
char chostNtpL[40]; char cportNtpL[6], cmtpntL[80]; char cuserL[40]; char csenL[40];
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
char chostNtpC[40]; char cportNtpC[6], cmtpntC[80]; char cuserC[40]; char csenC[40];

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
char chostNtpS[40]; char cportNtpS[6], cmtpntS[80]; char csendRev2[2]; char cuserS[40]; char csenS[40];
//SOURCE

AsyncWebServer server(80);
WiFiServer wifiServer(2101);
WiFiClient client = wifiServer.available();
NTRIPServer ntripL;
NTRIPSource ntripS;
NTRIPClient ntripclient;

void ext_com(bool state) {
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
              //sendLatLong = avancado["sendLatLong"].as<int>();
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
              //avancado
              JsonObject avancado = json["avancado"][0];
              Modo = strcpy(cModo, avancado["Modo"]);
              Perfil = strcpy(cPerfil, avancado["Perfil"]);
              Mac = strcpy(cMac, avancado["Mac"]);
              Conf = strcpy(cConf, avancado["Conf"]);
              voltConf = strcpy(cvoltConf, avancado["voltConf"]);
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
    avancado["voltConf"] = voltConf.c_str();
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

int Swifi = 0;
void WiFiEvent(WiFiEvent_t event) {
  /*Serial.printf("[WiFi-event] event: %d\n", event);*/

  switch (event) {
    case ARDUINO_EVENT_WIFI_READY:              /*Serial.println("WiFi interface ready"); */  break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:      /*Serial.println("Connected to access point");*/ Swifi = 1; break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:   /*Serial.println("Disconnected from WiFi access point");*/ Swifi = 2; break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:         /*Serial.print("Obtained IP address: ");Serial.println(WiFi.localIP());*/ Swifi = 1; break;
    default:                                    break;
  }
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}

void sanitizeURL(const char* url, char* cleanHost, size_t bufferSize) {
  const char* domain = url;

  // Skip "http://" or "https://"
  if (strncmp(domain, "http://", 7) == 0) {
    domain += 7;
  } else if (strncmp(domain, "https://", 8) == 0) {
    domain += 8;
  }

  // Find the end of the domain (stop at '/', '?', ':', etc.)
  const char* end = strpbrk(domain, "/?:"); 
  if (!end) end = domain + strlen(domain); // No delimiter found

  // Calculate length to copy
  size_t len = end - domain;

  // Ensure it fits in the buffer
  if (len >= bufferSize) len = bufferSize - 1;

  // Copy sanitized domain to `cleanHost`
  strncpy(cleanHost, domain, len);
  cleanHost[len] = '\0'; // Null-terminate
}

static char ipBuffer[16] = {0}; // "xxx.xxx.xxx.xxx\0"

char* DNSTranslation(const char* DNSInput) {
  static bool translation = true; // Only resolve once

  if (translation) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nWiFi failed!");
      return nullptr; // Indicate failure
    }

    Serial.println("\nWiFi connected!");

    char cleanHost[40];
    sanitizeURL(DNSInput, cleanHost, sizeof(cleanHost));

    IPAddress resolvedIP;
    if (WiFi.hostByName(cleanHost, resolvedIP)) {
      // Write IP directly to static buffer (no String)
      snprintf(ipBuffer, sizeof(ipBuffer), "%d.%d.%d.%d",
               resolvedIP[0], resolvedIP[1], resolvedIP[2], resolvedIP[3]);

      Serial.print("Resolved IP: ");
      Serial.println(ipBuffer);

      translation = false; // Prevent re-resolution
      return ipBuffer; // Return pointer to static buffer
    } else {
      Serial.println("Resolution failed!");
      return nullptr;
    }
  }

  // Return cached IP on subsequent calls
  return ipBuffer;
}

void setup() {
  if (!SPIFFS.begin(true)) {
    //handleSpiFFSError();
  }
  loaddata(BDR_data);
  pins_init();
  digitalWrite(LED1B,HIGH);
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, TX, RX);
  ext_com(1);
  //Serial.println("teste Serial0");
  //Serial1.println("teste Serial1");
  displaybdr.initDisplay();

  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFiEventId_t eventID = WiFi.onEvent(
    [](WiFiEvent_t event, WiFiEventInfo_t info) {
      //Serial.print("WiFi lost connection. Reason: ");
      //201 rede nao encontrada, 15 senha incorreta, 6 desconectou
      //Serial.println(info.wifi_sta_disconnected.reason);
      if (info.wifi_sta_disconnected.reason == 201) {
        Swifi = 3;
      } else if (info.wifi_sta_disconnected.reason == 15) {
        Swifi = 4;
      } else if (info.wifi_sta_disconnected.reason == 6) {
        Swifi = 5;
      }
    },
    WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED
  );
  if (!displaybdr.display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  displaybdr.display.clearDisplay();
  if (Conf == "1") {
      wifisetup();
      conf();
    } else if (Conf == "0" && Modo == "Local") {
      local();
    } else if (Conf == "0" && Modo == "Cliente") {
      cliente();
    } else if (Conf == "0" && Modo == "Source") {
      source();
    }
}

String ssid;
void wifisetup() {
  //Serial.println("onConf1 " + Conf);
  WiFi.mode(WIFI_AP);
  String macAddress = String(ESP.getEfuseMac());
  Mac = macAddress.substring(macAddress.length() - 4);
  ssid = "BDR-" + Mac;
  WiFi.softAP(ssid.c_str(), NULL, 7, 0, 5);
  saveConfigFile(BDR_data);
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
        voltConf = saidaTenL;
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
        voltConf = saidaTenC;
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
        voltConf = saidaTenS;
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
        ESP.restart();
    });
    server.begin();
}

void usbpd(int volt){
  switch(volt){
    case 5:
      digitalWrite(CF1,HIGH);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
    break;
    case 9:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
    break;
    case 12:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,HIGH);
    break;
    case 15:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,HIGH);
    break;
    case 20:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,LOW);
    break;
  }
}

int test_pd(){
  static int volt;
  static int state = 1;
  usbpd(5);
  volt = 5;
  //Serial.println(volt);
  usbpd(9);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 1;
    volt = 9;
    //Serial.println(volt);
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 1;
  }
  usbpd(12);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 2;
    volt = 12;
    //Serial.println(volt);
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 2;
  }
  usbpd(15);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 3;
    volt = 15;
    //Serial.println(volt);
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 3;
  }
  usbpd(20);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 4;
    volt = 20;
    //Serial.println(volt);
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 4;
  }
  if (Conf == "1" && (voltConf.toInt() > volt)) {
    digitalWrite(POUT,LOW);
    //Serial.print("POUT Low Conf: ");
    //Serial.print(voltConf.toInt());
    //Serial.print(" ");
    //Serial.print(volt);
    //digitalWrite(LED2R,LOW);
    //digitalWrite(LED2G,HIGH);
  } else if (Conf == "0" && Modo == "Local" && (saidaTenL.toInt() > volt)) {
    digitalWrite(POUT,LOW);
    //Serial.print("POUT Low Local: ");
    //Serial.print(voltConf.toInt());
    //Serial.print(" ");
    //Serial.print(volt);
    //digitalWrite(LED2R,LOW);
    //digitalWrite(LED2G,HIGH);
  } else if (Conf == "0" && Modo == "Cliente" && (saidaTenC.toInt()  > volt)) {
    digitalWrite(POUT,LOW);
    //Serial.print("POUT Low cliente: ");
    //Serial.print(voltConf.toInt());
    //Serial.print(" ");
    //Serial.print(volt);
    //digitalWrite(LED2R,LOW);
    //digitalWrite(LED2G,HIGH);
  } else if (Conf == "0" && Modo == "Source" && (saidaTenS.toInt() > volt)) {
    digitalWrite(POUT,LOW);
    //Serial.print("POUT Low source: ");
    //Serial.print(voltConf.toInt());
    //Serial.print(" ");
    //Serial.print(volt);
    //digitalWrite(LED2R,LOW);
    //digitalWrite(LED2G,HIGH);
  } else {
    //Serial.println(volt);
    digitalWrite(POUT,HIGH);
    //Serial.println("POUT High");
    //digitalWrite(LED2R,HIGH);
  }
  return state;
}

void LedFunc(int LED1, int LED2) { //0-preto, 1-vermelho, 2-verde, 3-azul, 4-amarelo, 5-rosa, 6-ciano, 7-branco
  digitalWrite(LED1R,LOW);
  digitalWrite(LED1G,LOW);
  digitalWrite(LED1B,LOW);
  digitalWrite(LED2R,LOW);
  digitalWrite(LED2G,LOW);
  digitalWrite(LED2B,LOW);
  switch(LED1) {
    case 0: //preto
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,LOW);
    break;
    case 1: //vermelho
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,LOW);
    break;
    case 2: //verde
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,LOW);
    break;
    case 3: //azul
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,HIGH);
    break;
    case 4: //amarelo
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,LOW);
    break;
    case 5: //rosa
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,HIGH);
    break;
    case 6: //ciano
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,HIGH);
    break;
    case 7: //branco
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,HIGH);
    break;
  }
  switch(LED2) {
    case 0: //preto
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,LOW);
    break;
    case 1: //vermelho
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,LOW);
    break;
    case 2: //verde
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,LOW);
    break;
    case 3: //azul
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,HIGH);
    break;
    case 4: //amarelo
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,LOW);
    break;
    case 5: //rosa
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,HIGH);
    break;
    case 6: //ciano
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,HIGH);
    break;
    case 7: //branco
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,HIGH);
    break;
  }
}

void LedBlink(int LED1, int LED2) {
  static int lastmillisled = 0;
  int currentmillisled = millis();
  if (currentmillisled - lastmillisled >= 500) {
    LedFunc(LED1, LED2);
    if (currentmillisled - lastmillisled >= 1000) {
      LedFunc(LED1, 0);
      lastmillisled = currentmillisled;
    }
  }
}

void LedConf() {
  static int RColor = 0;
  static int GColor = 0;
  static int BColor = 0;
  static int ColorIndex;

  static int lastmillisled = 0;
  int currentmillisled = millis();
  if (currentmillisled - lastmillisled > 500) {
    switch(ColorIndex) {
      case 0: // vermelho
        LedFunc(7, 1);
        ColorIndex = 1;
      break;
      case 1: //amarelo
        LedFunc(7, 4);
        ColorIndex = 2;
      break;
      case 2: //verde
        LedFunc(7, 2);
        ColorIndex = 3;
      break;
      case 3: //ciano
        LedFunc(7, 6);
        ColorIndex = 4;
      break;
      case 4: //azul
        LedFunc(7, 3);
        ColorIndex = 5;
      break;
      case 5: //rosa
        LedFunc(7, 5);
        ColorIndex = 6;
      break;
      case 6: //branco
        LedFunc(7, 7);
        ColorIndex = 0;
      break;
    }
    lastmillisled = currentmillisled;
  }
}

char timeCountVal[30] = "-- --:--";
void timeCount() {
  static unsigned long lastUpdate = 0;
  unsigned long currentMillis = millis();

  // Update display every second
  if (currentMillis - lastUpdate >= 1000) {
    lastUpdate = currentMillis;
    formatTime(currentMillis);
  }
}

void formatTime(unsigned long millisTime) {
  unsigned long totalSeconds = millisTime / 1000;
  
  // Calculate time components
  unsigned long days = totalSeconds / 86400;  // 86400 seconds = 1 day
  unsigned long hours = (totalSeconds % 86400) / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;

  // Format the output
  if (days > 0) {
    snprintf(timeCountVal, sizeof(timeCountVal), "%luD %02lu:%02luH/M", days, hours, minutes);
  } else {
    snprintf(timeCountVal, sizeof(timeCountVal), "%02lu:%02luH/M", hours, minutes);
  }
  
  //Serial.println(timeCountVal);
}

String CriaGGA(String lati, String logi, String alti, String tutc){
  String p = "GNGGA,";
  String p1 = "";
  String p2 = "";
  String newLati, NorS, EorW, newLong;
  float decimalCoord, minutos;
  int graus;
  for(int i = 0; i < 2; i++){
    if(i==0){
      decimalCoord = lati.toFloat();   
      graus = (int)decimalCoord;
      minutos = (abs(decimalCoord) - abs(graus)) * 60;
      char resultado[15]; // buffer para armazenar o resultado formatado
      // Formatar a string no formato desejado
      snprintf(resultado, sizeof(resultado), "%02d%09.6f", graus, minutos);
      String newResult = String(resultado);
      if (newResult.indexOf("-")<=0){
        NorS = "S";
        newLati = newResult.substring(newResult.indexOf("-")+1);
        p1 = "";
        p1 = newLati + String(",")+ NorS;
      }else{
        NorS = "N";
        p1 = "";
        p1 = resultado + String(",")+ NorS;
      }
    }else if(i==1){
      decimalCoord = logi.toFloat();   
      graus = (int)decimalCoord;
      minutos = (abs(decimalCoord) - abs(graus)) * 60;
      char resultado1[15]; // buffer para armazenar o resultado formatado
      // Formatar a string no formato desejado
      snprintf(resultado1, sizeof(resultado1), "%02d%09.6f", graus, minutos);
      String newResult1 = String(resultado1);
      if (newResult1.indexOf("-")<=0){
        EorW = "W";
        newLong = newResult1.substring(newResult1.indexOf("-")+1);
        p2 = "";
        p2 = newLong + String(",")+ EorW;
      }else{
        EorW = "E";
        p2 = "";
        p2 = resultado1 + String(",")+ EorW;
      }
    }
  }
  String newalt = alti.substring(0,6);
  p = p + tutc + String(".00,") + p1 + String(",0") + p2 + String(",1,12,1.2,")+ newalt + String(",M,,,,");
  int checksum = 0;
  for(int e = 0; e < p.length(); e++){
    checksum ^= p[e];
  }
  String checksumHex = String(checksum,HEX);
  if (checksumHex.length() < 2) {
    checksumHex = "0" + checksumHex;
  }
  checksumHex.toUpperCase();
  String GNGGA = "$" + p + "*" + checksumHex;
  return GNGGA;
}

int timer = 0;
int packageDisp = 0;
unsigned long previousMillis = 0;
int errorClient = 6; 
unsigned int readcount;
bool setupDone = false;
int cliente(const char* Network, const char* Password, char* Port, char* NTPHost, char* NTPUser, char* NTPPassword, const char* MACAdr, char* Latitude, char* Longitude, char* Altitude, char* Tempoutc, String SendLocation, char* MTPT){
  if (setupDone == false) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(Network, Password);
  static int i = 0;
  while (WiFi.status() != WL_CONNECTED&&i<50) {
    Serial.print(".");
    i++;
    delay(100);
  }
  if (WiFi.status() == WL_CONNECTED && DNSTranslation(NTPHost) != NULL) {
    NTPHost = DNSTranslation(NTPHost);
  }
  int iPORT = strtol(Port, NULL, 0);
  Serial.println("Requesting SourceTable.");
  //switch(client.reqSrcTbl(cHOST,iPORT,FMAC)){
  switch(ntripclient.reqRaw(NTPHost,iPORT,"",NTPUser,NTPPassword,"","",MACAdr)){
    case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
            Serial.println("SourceTable request error");
            errorClient = 1;
    break;
    case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
            Serial.println("SourceTable request error");
            errorClient = 2;
    break;
    case 4: Serial.print("tudo ok!");
            char buffer[512];
            delay(1000);
            while(ntripclient.available()){
              ntripclient.readLine(buffer,sizeof(buffer));
              Serial.print(buffer); 
            }
            Serial.println("Requesting SourceTable is OK\n");
            errorClient = 0;
    break;
    case 5: Serial.println("Servidor não responde 1");
            Serial.println("SourceTable request error");
            errorClient = 4;
    break;
    case 6: Serial.println("Mount Point incorreto");
            errorClient = 5;
    break;
  }
  Serial.println("Requesting MountPoint's Raw data");     
  if(SendLocation=="S"){
    if(Latitude!=""&&Longitude!=""&&Altitude!=""&&Tempoutc!=""){
      String nmeaGGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
      switch(ntripclient.reqRaw(NTPHost,iPORT,MTPT,NTPHost,NTPPassword,nmeaGGA,SendLocation,MACAdr)){
        case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
                ntripclient.stop();
                errorClient = 1;
        break;
        case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
                ntripclient.stop();
                errorClient = 2;
        break;
        case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(NTPUser);Serial.print(":");Serial.println(NTPPassword);
                Serial.println("unable to connect with ntrip caster");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 3;
        break;
        case 4: Serial.print("tudo ok!");
                errorClient = 0;
        break;
        case 5: Serial.println("Servidor não responde 2");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 4;
        break;
        case 6: Serial.println("Mount Point incorreto");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 5;
        break;
      }
    }
  }else if (SendLocation=="N"){
    switch(ntripclient.reqRaw(NTPHost,iPORT,MTPT,NTPUser,NTPPassword,"","",MACAdr)){
      case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
              ntripclient.stop();
              errorClient = 1;
      break;
      case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
              ntripclient.stop();
              errorClient = 2;
      break;
      case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(NTPHost);Serial.print(":");Serial.println(NTPPassword);
              Serial.println("unable to connect with ntrip caster");
              ntripclient.stop();
              errorClient = 3;
      break;
      case 4: Serial.print("tudo ok!");
              errorClient = 0;
      break;
      case 5: Serial.println("Servidor não responde 3");
              ntripclient.stop();
              errorClient = 4;
      break;
      case 6: Serial.println("Mount Point incorreto");
              ntripclient.stop();
              errorClient = 5;
      break;
      }
    }
    if (i >= 50){
      errorClient = 4;
    }
    setupDone = true;
  }
  if (errorClient == 0) {
    String GGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
    String enviaGGA = GGA + "\r\n";
    //Serial.println("ntrip avail: " + ntripclient.available());
      //Serial.println("while 1");
      unsigned long currentMillis = millis();
      static int readcount_disp;
    if (currentMillis - previousMillis >= 10) {
      if(ntripclient.available()){
        currentMillis = millis();
        char ch0[5000];
        readcount = ntripclient.readBytes(ch0, ntripclient.available());
        packageDisp = readcount;
        //Serial.println(readcount);
        readcount = 0;
        previousMillis = currentMillis;
        timer = 0;
        timeCount();
      } else {
        unsigned long currentMillisTimer = millis();
        if (currentMillisTimer - previousMillis >= 1000) {
          previousMillis = currentMillisTimer;
          if (timer < 99) {
            timer++;
            if (timer > 3) {
              readcount_disp = 0;
              packageDisp = 0;
            }
            if (timer > 20) {
              ntripclient.sendgga(enviaGGA);
              previousMillis = currentMillisTimer;
            }
          } else {
            setupDone = false;
            snprintf(timeCountVal, sizeof(timeCountVal), "-- --:--");
            packageDisp = 0;
            timer = 0;
            errorClient = 4;
            return false;
          }
        }
      }
    }
  }
  //milli_inicio = millis();
  return true;
}

int currentPage = 0;
void conf() {
  while(true) {
    LedConf();
    if (digitalRead(PAGE) == HIGH) {
      currentPage = currentPage + 1;
      delay(200);
    }
    if (currentPage==1) { 
      if (digitalRead(ENTER) == HIGH) {
        Conf = "0";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    if (currentPage > 3) {
        currentPage = 0;
    }
    displaybdr.display_config(currentPage, WiFi.softAPgetStationNum(), voltConf.toInt(), test_pd(), "BDR02", "V1.0", String(ESP.getEfuseMac()));
    //displaybdr.page_client("page", "statusOUT", "Swifi", "Sntrip", "Ssaida", "PDvolts", "Wrede", "Wip", "Nrede", "rssi", "Nlaten",
    //"uptim", "lat", "lon", "alt", "pre", "utc", "tensao", "pacot", "Slaten", 
    //"modelo", "firmware", "serial");
    //displaybdr.page_local("page", "statusOUT", "Ssaida", "PDvolts", "Wssid", "Wpass", "Nhost", "Nmtpt", "Nuser", "Npass",
    //"Wusers", "Nport", "baud", "pari", "data", "stop", "flow", "tensao", "pacot", "Slaten", "modelo", "firmware", "serial");
    //displaybdr.page_source("page", "statusOUT", "Swifi", "Sntrip", "Ssaida", "PDvolts", "Wrede", "Wip", "Nrede", "rssi", "Nlaten",
    //"uptim", "tensao", "pacot", "Slaten", "modelo", "firmware", "serial");
    displaybdr.display.display();
  }
}

int sourceError = 6;
int NTRIPSourceStart(Stream& serialPort, WiFiClient& client, int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, String rev, char* NTPUser, char* NTPPass, char* srcSTR) {
  //if (setupResult != 1) {
  //  Serial.println("Setup failed. Exiting...");
  //  return -1;  // Early exit if setup fails
  //}
  static int setupResult;
  static bool substationExecuted = false;
  if (!substationExecuted) {
    ntripS.NTRIPSourceSetup(Network, Password);
    if (WiFi.status() == WL_CONNECTED && DNSTranslation(Host) != NULL) {
      Host = DNSTranslation(Host);
    }
    //Serial.println("Subscribing MountPoint is OK");
    sourceError = ntripS.subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR);
    if (sourceError == 0 || sourceError == 2) {
      setupResult = 1;
      //Serial.println("setup success");
    } else if (sourceError == 1 || sourceError == 3 || sourceError == 5) {
      setupResult = 0;
      //Serial.println("substation fail");
    }
    substationExecuted = true;
  }
  if (setupResult != 1) {
    //Serial.println(setupResult);
    return -1;
  }
  char ch[512];
  int readcount = 0;
  if (client.connected()) {
    while (serialPort.available()) {
      while (serialPort.available()) {
        ch[readcount] = serialPort.read();
        readcount++;
        if (readcount > 511)break;
        //Serial.println(Serial2.read());
      }//buffering
      //Serial.print("readcount: ");Serial.println(readcount);
      packageDisp = readcount;
      client.write((uint8_t*)ch, readcount);
      //dataPass = readcount;
      readcount = 0;
      memset(ch,' ',readcount*sizeof(char));
      timer = 0;
    }
    timeCount();
    unsigned long currentMillisTimer = millis();
    if (currentMillisTimer - previousMillis >= 1000) {
      previousMillis = currentMillisTimer;
      if (timer < 99) {
        timer++;
        if (timer > 3) {
          packageDisp = 0;
        }
        if (timer > 20) {
          snprintf(timeCountVal, sizeof(timeCountVal), "-- --:--");
          previousMillis = currentMillisTimer;
        }
      }
    }
  } else {
    client.stop();
    //Serial.println("reconnect");
    //Serial.println("Subscribing MountPoint");
    static int previousAttempt = 0;
    int currentAttempt = millis();
    if (currentAttempt - previousAttempt > 60000) {
      if (WiFi.status() != WL_CONNECTED) {
        ntripS.NTRIPSourceSetup(Network, Password);
      }
      sourceError = ntripS.subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR);
      previousAttempt = currentAttempt;
      //Serial.println("erro, tentando novamente no proximo minuto");
    }
    packageDisp = 0;
    snprintf(timeCountVal, sizeof(timeCountVal), "-- --:--");
    timer = 0;
  }
  return true;
}

int localError;
unsigned long previousMillisLocal = 0;
int NTRIPLocal(WiFiClient& client, Stream& serialPort, char* User, char* Pass, char* Mountpoint){
  char client_data[1024];
  while(client.connected()){
    int counter = 0; 
    //Serial.println("while1");
    while (client.available()>0){  
      client_data[counter] = client.read();
      counter++;
      if(client.available()==0){
        String client_data_s = String(client_data);
        Serial.println(client_data_s.indexOf("GET"));
        Serial.println(client_data_s);
        memset(client_data,' ',sizeof(client_data)*sizeof(char));
        if(client_data_s.indexOf("GET")==0){
          
          int client_status = ntripL.analize_data(client_data_s,User,Pass,Mountpoint);
          if(client_status!=0){
            if(client_status &(1 << 4)){
              //solicitando mntp
              client.println("SOURCETABLE 200 OK");
              client.println(ntripL.scrtbl("ntripteste",WiFi.softAPIP().toString(),2101));
              client.stop();
              localError = 0;
              return 6;
            }
            Serial.println(client_status); 
            if(client_status &(1 << 1)||client_status &(1 << 2)){
              // tudo ok!
              if(client_status &(1 << 1)){
                //ntrip 1.0
                client.println("ICY 200 OK");
                localError = 5;
              }
              if(client_status &(1 << 2)){
                //ntrip 2.0
                client.println("HTTP/1.1 200 OK");
                localError = 5;
              }
              if(client_status &(1 << 5)){
                client.println("HTTP/1.1 401");
                client.stop();
                localError = 1;
                return 2;
                //user incorreto
              }
              if(client_status &(1 << 6)){
                client.println("HTTP/1.1 401");
                client.stop();
                localError = 2;
                return 3;
                //pass incorreto
              }
              if(client_status &(1 << 3)){
                client.println("HTTP/1.1 404");
                client.stop();
                localError = 3;
                return 4;
                //mntp incorreto   
              }
              Serial.println("tudo ok!");
              while(client.connected()){
                if (digitalRead(PAGE) == HIGH) {
                  //Serial1.println("teste");
                  currentPage = currentPage + 1;
                  delay(200);
                }
                if (digitalRead(ENTER) == HIGH) {
                  Conf = "1";
                  saveConfigFile(BDR_data);
                  ESP.restart();
                }
                if (currentPage > 7) {
                    currentPage = 0;
                }
                if(digitalRead(DET)==LOW) {
                  portConnected = 1;
                } else {
                  portConnected = 0;
                  //Seri
                }
                LedLocal(localError);
                displaybdr.page_local(currentPage, csaidaTenL, localError, portConnected, test_pd(), cssidL, csenwiL, chostNtpL, cmtpntL, cuserL, csenL,
                String(WiFi.softAPgetStationNum()), cportNtpL, cbaudrateL, cparidadeL, cdatbitL, cbitsParaL, ccontrFluxL, csaidaTenL,
                String(packageDisp), (String(timer) + "S"), "BDR02", "V1.0", String(ESP.getEfuseMac()));
                displaybdr.display.display();
                if(serialPort.available()) {  
                  previousMillis = millis();
                  timer = 0;
                  char ch_ap[1024];
                  int readcount_ap = 0;
                  Serial.println(serialPort.available());
                  while (serialPort.available()) {
                    ch_ap[readcount_ap] = serialPort.read();
                    readcount_ap++;
                    if (readcount_ap > 511)break;
                    //Serial.println("while4");
                  }//buffering
                  
                  client.write((uint8_t*)ch_ap, readcount_ap);
                  Serial.println(readcount_ap);
                  packageDisp = readcount_ap;
                }
                 else {
                  unsigned long currentMillisLocal = millis();
                  if (currentMillisLocal - previousMillisLocal >= 1000) {
                    previousMillisLocal = currentMillisLocal;
                    if (timer < 99) {
                      timer++;
                      if (timer > 3) {
                        packageDisp = 0;
                      }
                    } else {
                      timer = 0;
                      localError = 0;
                      return 1;
                    }
                  }
                }
                if (client.available()!=0) {
                  break;
                }
                //Serial.println(client.available());
                //Serial.println("while3");
              }
              packageDisp = 0;
              localError = 0;
              return 1;
            }
          }else{
            //trata erro!!! nao http
            client.println("HTTP/1.1 400");
            client.stop();
            localError = 4;
            return 5;
          }
        }else{
          //client.println("HTTP/1.1 502");
          client.stop();
          localError = 0;
          return 0;
        }
      }
      //Serial.println("while2");
    }
  } 
}

void local() {
  //Serial.println("local");
  ntripL.NTRIPLocalSetup(cssidL, NULL);
  wifiServer.begin();
  while(true) {
    LedLocal(localError);
    if (digitalRead(PAGE) == HIGH) {
      //Serial1.println("teste");
      currentPage = currentPage + 1;
      delay(200);
    }
    if (currentPage==5) {
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    if (currentPage > 7) {
        currentPage = 0;
    }
    //Serial.println("Serial avail: "); Serial.println(Serial1.read());
    if(digitalRead(DET)==LOW) {
      portConnected = 1;
    } else {
      portConnected = 0;
    }
    displaybdr.page_local(currentPage, csaidaTenL, localError, portConnected, test_pd(), cssidL, csenwiL, chostNtpL, cmtpntL, cuserL, csenL,
                String(WiFi.softAPgetStationNum()), cportNtpL, cbaudrateL, cparidadeL, cdatbitL, cbitsParaL, ccontrFluxL, csaidaTenL,
                "0", "--", "BDR02", "V1.0", String(ESP.getEfuseMac()));
    displaybdr.display.display();
    WiFiClient client = wifiServer.available();
    if(client){
      NTRIPLocal(client, Serial1, cuserL, csenL, cssidL);
    }
  }
}

void LedLocal(int err) {
  switch(err) {
    case 0:
      LedFunc(2, 0);
    break;
    case 1:
      LedBlink(1, 1);
    break;
    case 2:
      LedBlink(1, 4);
    break;
    case 3:
      LedBlink(1, 6);
    break;
    case 4:
      LedBlink(1, 5);
    break;
    case 5:
    if (packageDisp > 0) {
      LedFunc(2, 3);
    } else if (packageDisp == 0) {
      LedFunc(2, 0);
    }
  }
}

void cliente() {
  while(true) {
    LedClientSource(errorClient);
    if (digitalRead(PAGE) == HIGH) {
      currentPage = currentPage + 1;
      delay(200);
    }
    if (currentPage==5) {
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    if (currentPage > 7) {
        currentPage = 0;
    }
    if(digitalRead(DET)==LOW) {
      portConnected = 1;
    } else {
      portConnected = 0;
    }
    displaybdr.page_client(currentPage, csaidaTenC, Swifi, errorClient, portConnected, test_pd(), cssidC, (WiFi.localIP().toString()).c_str(), chostNtpC,
    WiFi.RSSI(), (String(timer) + "S"), timeCountVal, sendLatLong, clat, clon, calt, cpre, cutc, csaidaTenC, String(packageDisp), (String(timer) + "S"),
    "BDR02", "V1.0", String(ESP.getEfuseMac()));
    displaybdr.display.display();
    //cliente(cssidC, csenwiC, "2101", "200.135.184.95", "agrimensura", "ifscntrip", "BTX-0553", "17.0000", "48.0000" , "18.000", "00:00:00", "N", "IFSC0");
    cliente(cssidC, csenwiC, cportNtpC, chostNtpC, cuserC, csenC, ssid.c_str(), clat, clon , calt, cutc, sendLatLong, cmtpntC);
  }
}

void LedClientSource(int err) {
  if (Swifi == 1||Swifi == 0) {
    switch(err) {
      case 0:
        if (packageDisp > 0) {
          LedFunc(2, 3);
        } else if (packageDisp == 0) {
          LedFunc(2, 0);
        }
      break;
      case 1: 
        LedBlink(1, 1);
      break;
      case 2: 
        LedBlink(1, 3);
      break;
      case 3:
        LedBlink(1, 4);
      break;
      case 4:
        LedBlink(1, 5);
      break;
      case 5:
        LedBlink(1, 6);
      break;
      case 6:
        LedBlink(1, 7);
      break;
    }
  } else if (Swifi == 2||Swifi == 5) {
    LedBlink(4, 1);
  } else if (Swifi == 3) {
    LedBlink(4, 4);
  } else if (Swifi == 4) {
    LedBlink(4, 6);
  }
}

void source() {
  while(true) {
    LedClientSource(sourceError);
    if (digitalRead(PAGE) == HIGH) {
      currentPage = currentPage + 1;
      delay(200);
    }
    if (currentPage==4) {
      if (digitalRead(ENTER) == HIGH) {
        Conf = "1";
        saveConfigFile(BDR_data);
        ESP.restart();
      }
    }
    if (currentPage > 6) {
        currentPage = 0;
    }
    if(digitalRead(DET)==LOW) {
      portConnected = 1;
    } else {
      portConnected = 0;
    }
    displaybdr.page_source(currentPage, csaidaTenS, Swifi, sourceError, portConnected, test_pd(), cssidS, (WiFi.localIP().toString()).c_str(),
    chostNtpS, WiFi.RSSI(), (String(timer) + "S"), timeCountVal, csaidaTenS, String(packageDisp), (String(timer) + "S"), "BDR02", "V1.0", String(ESP.getEfuseMac()));
    displaybdr.display.display();
    //NTRIPSourceStart(Serial1, client, 2101, "HugenPLUS", "H32851112", "192.168.15.16", "juca", sendRev2, "jucas", "jucapass", "");
    NTRIPSourceStart(Serial1, client, portNtpS.toInt(), cssidS, csenwiS, chostNtpS, cmtpntS, sendRev2, cuserS, csenS, "");
  }
}

void loop () {
  //Serial.println("teste Serial0");
  //Serial1.println("teste Serial1");
  //delay(1000);
}