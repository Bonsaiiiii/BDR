#include <WiFi.h>

#define DEBUG 1

String ERRW;
bool flag_erro, flag_check_wifi;
unsigned long last_millis;
unsigned long previousMillis = 0;
unsigned long interval = 30000;

const char* hostname = "BDR-30:45";

const char* ssid = "bisnaguinha5000";
const char* password = "12345678";
int Swifi;
int tentativas;
bool F_stop_reconect;

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
    Swifi = 4;
  }
}

void sta_wifi(){
  last_millis = millis();
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(wifi_desconectado, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

void ap_wifi(){
  last_millis = millis();
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.onEvent(WiFiEvent);
  //WiFi.onEvent(wifi_desconectado, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
}

bool check_wifi (unsigned long interval, unsigned long retries){
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
      tentativas ++;
    } else {
      WiFi.disconnect(true);
      return 0;
    } 
  }else if (WiFi.status() == WL_CONNECTED){
    tentativas = 0;
  }
  return 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);
  Serial.println("teste de wifi");
  sta_wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
 if (Swifi == 1 || Swifi == 2){
  check_wifi(10000, 2);
  
 }
 delay(10);
}
