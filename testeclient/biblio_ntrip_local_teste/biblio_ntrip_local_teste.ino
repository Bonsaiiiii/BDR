#include "WiFi.h"
#include "HardwareSerial.h"
#include "NTRIPClient.h"

NTRIPClient ntripclient;
//WiFiClient client = ntripServer.available();

void setup() {
  /*Serial.begin(115200);
  delay(100);
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ntripteste", NULL, 7, 0, 10);   //launch the access point
  ntripServer.begin();
  Serial.println("setup concluido");
  */
  Serial.begin(115200);
  Serial.println("setup ok");
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

unsigned long previousMillis = 0;
const long interval = 10;
int errorClient = 0; 
unsigned int readcount;
bool setupDone = false;
int cliente(const char* Network, const char* Password, char* Port, char* NTPHost, char* NTPUser, char* NTPPassword, char* MACAdr, char* Latitude, char* Longitude, char* Altitude, char* Tempoutc, char* SendLocation, char* MTPT){
  if (setupDone == false) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(Network, Password);
  int i = 0;
  //unsigned long currentMillis;
  //unsigned long atualMillis;
  //unsigned long milli_inicio;
  bool flag_erro = false;
  int ERRW;
  static int errorNum;
  while (WiFi.status() != WL_CONNECTED&&flag_erro==false) {
    delay(100);
    switch(WiFi.status()){
        case WL_CONNECTED:
          ERRW = 0;
          errorClient = 0;
        break;
        case WL_NO_SSID_AVAIL:
          ERRW = 1;
          Serial.println("NOT AVAIL");
          errorClient = 1;
        break;
      }
    if(i>50){
      flag_erro = true;
    }
    i++;
  }
  if(ERRW!=1&&flag_erro==true) {
    ERRW = 2; // Senha incorreta
    Serial.println("No pass");
    return 2;
    errorClient = 2;
  }  
  if (flag_erro&&ERRW==1) {
    int t=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
      Serial.println("Rede não encontrada/ login invalido");
      delay(100);
      //ESP.restart();
      }else{
        t++;
      }
    }
  }
  else if (flag_erro&&ERRW==2) {
    int t=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting to WiFi..");
      if(t>5){
      Serial.println("Senha incorreta");
      delay(100);
      //ESP.restart();
      }else{
        t++;
      }
    }
  }
  //Serial.println(WiFi.localIP());
  int iPORT = strtol(Port, NULL, 0);
  Serial.println("Requesting SourceTable.");
  //switch(client.reqSrcTbl(cHOST,iPORT,FMAC)){
  switch(ntripclient.reqRaw(NTPHost,iPORT,"",NTPUser,NTPPassword,"","",MACAdr)){
    case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
            Serial.println("SourceTable request error");
            errorClient = 3;
    break;
    case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
            Serial.println("SourceTable request error");
            errorClient = 4;
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
            errorClient = 6;
    break;
    case 6: Serial.println("Mount Point incorreto");
            errorClient = 7;
    break;
  }
  Serial.println("Requesting MountPoint's Raw data");     
  if(SendLocation=="S"){
    if(Latitude!=""&&Longitude!=""&&Altitude!=""&&Tempoutc!=""){
      String nmeaGGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
      switch(ntripclient.reqRaw(NTPHost,iPORT,MTPT,NTPHost,NTPPassword,nmeaGGA,SendLocation,MACAdr)){
        case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
                ntripclient.stop();
                //ESP.restart();
                errorClient = 3;
        break;
        case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 4;
        break;
        case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(NTPUser);Serial.print(":");Serial.println(NTPPassword);
                Serial.println("unable to connect with ntrip caster");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 5;
        break;
        case 4: Serial.print("tudo ok!");
                errorClient = 0;
        break;
        case 5: Serial.println("Servidor não responde 2");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 6;
        break;
        case 6: Serial.println("Mount Point incorreto");
                ntripclient.stop();
                //ESP.restart();
                errorClient = 7;
        break;
      }
    }
  }else if (SendLocation=="N"){
    switch(ntripclient.reqRaw(NTPHost,iPORT,MTPT,NTPUser,NTPPassword,"","",MACAdr)){
      case 1: Serial.print("Não foi possivel conectar ao ");Serial.print(NTPHost);Serial.print(":");Serial.println(iPORT);
              ntripclient.stop();
              //ESP.restart();
              errorClient = 3;
      break;
      case 2: Serial.println("Não ouve resposta do servidor ou resposta nao compreendida");
              ntripclient.stop();
              //ESP.restart();
              errorClient = 4;
      break;
      case 3: Serial.print("Usuario ou senha invalidos: ");Serial.print(NTPHost);Serial.print(":");Serial.println(NTPPassword);
              Serial.println("unable to connect with ntrip caster");
              ntripclient.stop();
              //ESP.restart();
              errorClient = 5;
      break;
      case 4: Serial.print("tudo ok!");
              errorClient = 0;
      break;
      case 5: Serial.println("Servidor não responde 3");
              ntripclient.stop();
              //ESP.restart();
              errorClient = 6;
      break;
      case 6: Serial.println("Mount Point incorreto");
              ntripclient.stop();
              //ESP.restart();
              errorClient = 7;
      break;
    }
  }
  }
  if (errorClient == 0) {
    String GGA = CriaGGA(Latitude,Longitude,Altitude,Tempoutc);
    String enviaGGA = GGA + "\r\n";
    //Serial.println("ntrip avail: " + ntripclient.available());
      //Serial.println("while 1");
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
      if(ntripclient.available()){
        currentMillis = millis();
        char ch0[5000];
        readcount = ntripclient.readBytes(ch0, ntripclient.available());
        Serial.println(readcount);
        readcount = 0;
        previousMillis = currentMillis;
      }
      if(currentMillis - previousMillis > 20000){  
        ntripclient.sendgga(enviaGGA);
        previousMillis = currentMillis;
      }
      
      /*if(atualMillis - currentMillis > 30000){
        for(int n=0; n<2; n++){
          Serial.println("milis");
        }
        ntripclient.stop();
        ESP.restart();
      } */
    }
    setupDone = true;
  }
  //milli_inicio = millis();
  return true;
}

int getErrorClient() {
  return errorClient;
}

void loop() {
  cliente("HugenPLUS", "H32851112", "2101", "200.135.184.95", "agrimensura", "ifscntrip", "BTX-0553", "17.0000", "48.0000" , "18.000", "00:00:00", "N", "IFSC0");
}
