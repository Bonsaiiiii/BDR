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

String ssid;
String password;
String SBAUD = "115200";
const char* mountpoint = "BDR0";
const char* usuario = "HGNP";
const char* senha  = "0102";
uint16_t port = 2101;
String Conf; char cConf[2];
bool flag_erro = false;
int estado = 0;

//GERAL
String Modo, Perfil, Mac, wifiEr;
char cModo[7]; char cPerfil[20]; char cMac[6]; char cwifiEr[2];

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
WiFiServer wifiServer(2101);
WiFiClient client = wifiServer.available();

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

void setup() {
  if (!SPIFFS.begin(true)) {
    //handleSpiFFSError();
  }
  loaddata(BDR_data);
  Serial.begin(115200);
  Serial.println("ConfSetup " + Conf);

  if (Conf == "1") {
      wifisetup();
    } else if (Conf == "0" && Modo == "Local") {
      local();
    } else if (Conf == "0" && Modo == "Cliente") {
      cliente();
    } else if (Conf == "0" && Modo == "Source") {
      source();
    }
}

void wifisetup() {
  Serial.println("onConf1 " + Conf);
      WiFi.mode(WIFI_AP);
      WiFi.softAP("BDR-", NULL, 7, 0, 5);
      String macAddress = WiFi.softAPmacAddress();
      String lastFourDigits = macAddress.substring(macAddress.length() - 5);
      macAddress.replace(":", "");
      password = macAddress.substring(macAddress.length() - 8);
      ssid = ("BDR-" + lastFourDigits);
      WiFi.softAP(ssid.c_str(), password, 7, 0, 5);
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
          Serial.println(wifiEr);
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
            Conf = "0";
            saveConfigFile(BDR_data);
            request->send(200);
            ESP.restart();
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
            Conf = "0";
            saveConfigFile(BDR_data);
            request->send(200);
            ESP.restart();
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
            saveConfigFile(BDR_data);
            request->send(200);
        });

      saveConfigFile(BDR_data);
      server.begin();
}

void local() {
  Serial.println("Local Mode");
  Serial.println(ssidL + " " + senwiL);
  Serial.println("Onlocal " + Conf);
  WiFi.softAP(ssidL, senwiL);
  delay(100);
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  delay(100);
  //wifiServer.begin();
  while(true){
    Serial.println("executando o local");
    delay(1000);
  }
}

void cliente() {
  Serial.println("Cliente Mode");
  Serial.println(ssidC + " " + senwiC);
  Serial.println("OnCliente " + Conf);
  WiFi.mode(WIFI_STA);
  WiFi.begin(cssidC, csenwiC);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED&&flag_erro==false) {
    delay(100);
    switch(WiFi.status()){
        case WL_CONNECTED:
        if(estado!=1){
          wifiEr = "0";  // Conectado com sucesso
          saveConfigFile(BDR_data);
        }
        break;
        case WL_NO_SSID_AVAIL:
        if(estado!=2){
          wifiEr = "1";  // Rede não encontrada/ Login inválido
          saveConfigFile(BDR_data);
        }
        break;
      }
    if(i>50){
      flag_erro = true;
    }
    i++;
  }
  if(wifiEr!="1"&&flag_erro==true) {
    wifiEr = "2"; // Senha incorreta
    saveConfigFile(BDR_data);
  }
  if (flag_erro&&wifiEr=="1") {
    int t = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
        for(int wait=0;wait<10;wait++){
        // handle connectign to wifi
      }
      Conf = "1";
      saveConfigFile(BDR_data);
      delay(100);
      ESP.restart();
      }else{
        t++;
      }
    }
  }
  else if (flag_erro&&wifiEr=="2") {
    int t=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
        for(int wait=0;wait<10;wait++){
       
      }
      Conf = "1";
      saveConfigFile(BDR_data);
      delay(100);
      ESP.restart();
      }else{
        t++;
      }
    }
  }
}

void source() {
  Serial.println("Cliente Mode");
  Serial.println(ssidS + " " + senwiS);
  Serial.println("OnCliente " + Conf);
  WiFi.mode(WIFI_STA);
  WiFi.begin(cssidS, csenwiS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED&&flag_erro==false) {
    delay(100);
    switch(WiFi.status()){
        case WL_CONNECTED:
        if(estado!=1){
          wifiEr = "0";  // Conectado com sucesso
          saveConfigFile(BDR_data);
        }
        break;
        case WL_NO_SSID_AVAIL:
        if(estado!=2){
          wifiEr = "1";  // Rede não encontrada/ Login inválido
          saveConfigFile(BDR_data);
        }
        break;
      }
    if(i>50){
      flag_erro = true;
    }
    i++;
  }
  if(wifiEr!="1"&&flag_erro==true) {
    wifiEr = "2"; // Senha incorreta
    saveConfigFile(BDR_data);
  }
  if (flag_erro&&wifiEr=="1") {
    int t = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
        for(int wait=0;wait<10;wait++){
        // handle connectign to wifi
      }
      Conf = "1";
      saveConfigFile(BDR_data);
      delay(100);
      ESP.restart();
      }else{
        t++;
      }
    }
  }
  else if (flag_erro&&wifiEr=="2") {
    int t=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
        for(int wait=0;wait<10;wait++){
       
      }
      Conf = "1";
      saveConfigFile(BDR_data);
      delay(100);
      ESP.restart();
      }else{
        t++;
      }
    }
  }
}

void loop() {
  
} 
