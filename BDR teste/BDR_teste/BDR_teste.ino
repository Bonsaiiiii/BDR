#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"

#define BDR_data "/bdrjson.json"

#define size_bdr 2048

//GERAL
int conf_bdr = 0;
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

bool loaddata(String filename) {
  if (SPIFFS.begin(false) || SPIFFS.begin(true)) {
    if (SPIFFS.exists(filename)) {
      if (filename == "/bdrjson.json") {
        File configFile = SPIFFS.open(filename, "r");
        if (configFile) {
          StaticJsonDocument<size_bdr> json;
          DeserializationError error = deserializeJson(json, configFile);
          if (!error) {
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
            return true;
          } else {
            Serial.println("Failed to load bdrjson.json");
          }
        } else {
          Serial.println("Failed to load bdrjson.json");
        }
      } else {
        Serial.println("filename dont match");
      }

    }
  } else {
    Serial.println("Failed to mount FS");
  }
  return false;
}

void saveConfigFile(String filename) {
  if (filename == "/bdrjson.json") {
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
    File configFile = SPIFFS.open(filename, "w");
    if (!configFile) {
      Serial.println("failed to open bdrjson.json file for writing");
    }
    if (serializeJson(json, configFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    configFile.close();
  }
}

void setup() {
  loaddata(BDR_data);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("BDR", NULL, 7, 0, 10);
  Serial.println(WiFi.softAPmacAddress());
  String macAddress = WiFi.softAPmacAddress();
  macAddress.replace(":", "");
  String lastFourDigits = macAddress.substring(macAddress.length() - 4);
  String ssid = "BDR_" + lastFourDigits;
  Mac = lastFourDigits;
  WiFi.disconnect();
  WiFi.softAP(ssid.c_str(), NULL, 7, 0, 10);
  Serial.println("Wait 100 ms for AP_START...");
  delay(100);
  Serial.println(WiFi.softAPmacAddress());
  saveConfigFile(BDR_data);

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/", String(), false);
  });
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(204);
  });
  server.on("/bdrjson.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bdrjson.json", "application/json");
    Serial.println("Json Enviado");
  });
  server.on("/localbdr", HTTP_POST, [](AsyncWebServerRequest * request) {
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
  server.on("/clientebdr", HTTP_POST, [](AsyncWebServerRequest * request) {
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
  server.on("/sourcebdr", HTTP_POST, [](AsyncWebServerRequest * request) {
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

  server.on("/config", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Received POST request");
    saveConfigFile(BDR_data);
    request->send(200);
  });
  server.begin();
  Serial.println("tudo certo");
}

void loop() {

}
