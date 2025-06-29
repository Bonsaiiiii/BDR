/*#include <NTRIPServer.h>

NTRIPServer ntrip;

void setup() {
    ntrip.begin("ntripteste", "user", "passw", "ntripteste");
}

void loop() {
    ntrip.handle();
} */

#include "WiFi.h"
#include "HardwareSerial.h"
#include "NTRIPServer.h"


WiFiServer ntripServer(2101);
NTRIPServer ntrip;
//WiFiClient client = ntripServer.available();

/*
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

int analize_data(String data, String user, String password, String mountpoint){
  int state = 0;
  char base64[50];
  unsigned char string[50];
  int posicao_http = data.indexOf("HTTP");
  int posicao_rn = data.indexOf("\r\n");
  int posicao_bara = data.indexOf("/");
  int posicao_basic = data.indexOf(": Basic") + 8;
  if (posicao_http < 0){
    return 0; //ERRO1 
  }
  String versao_http = data.substring(posicao_http, posicao_rn);
  if(versao_http == "HTTP/1.0"){
    state |= 1 << 1;
  }
  if(versao_http == "HTTP/1.1"){
    state |= 1 << 2;
  }
  String cliente_mountpoint = data.substring(posicao_bara,posicao_http-1);
  if(cliente_mountpoint == (String("/") + mountpoint)){
    state |= 1 << 3; 
  }
  if(cliente_mountpoint == "/SOURCETABLE.TXT " || cliente_mountpoint == "/"){
    state |= 1 << 4;
  }
  String usuario_encoded = data.substring(posicao_basic);
  usuario_encoded.toCharArray(base64,50);
  unsigned int string_length = decode_base64((unsigned char *)base64, string);
  string[string_length] = '\0';
  String usuario_decoded = String((char *)string);
  int separador = usuario_decoded.indexOf(":");
  if(usuario_decoded.substring(0,separador) == user){
    state |= 1 << 5;
  }
  if(usuario_decoded.substring(separador+1) == password){
    state |= 1 << 6;
  }
  return state;
}

void NTRIPLocalSetup(char* Network, char* Password) {
  Serial.begin(115200);
  delay(100);
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(Network, Password, 7, 0, 10);
  Serial.println("setup concluido");
} */

int NTRIPLocal(WiFiClient& client, Stream& serialPort, char* User, char* Pass, char* Mountpoint){
  char client_data[1024];
  if(client.connected()){
    static int counter = 0; 
    while (client.available()>0){  
      client_data[counter] = client.read();
      counter++;
      if(client.available()==0){
        String client_data_s = String(client_data);
        Serial.println(client_data_s.indexOf("GET"));
        Serial.println(client_data_s);
        memset(client_data,' ',sizeof(client_data)*sizeof(char));
        if(client_data_s.indexOf("GET")==0){
          
          int client_status = ntrip.analize_data(client_data_s,User,Pass,Mountpoint);
          if(client_status!=0){
            if(client_status &(1 << 4)){
              //solicitando mntp
              client.println("SOURCETABLE 200 OK");
              client.println(ntrip.scrtbl("ntripteste",WiFi.softAPIP().toString(),2101));
              client.stop();
              return 6;
            }
            Serial.println(client_status); 
            if(client_status &(1 << 1)||client_status &(1 << 2)){
              // tudo ok!
              if(client_status &(1 << 1)){
                //ntrip 1.0
                client.println("ICY 200 OK");
              }
              if(client_status &(1 << 2)){
                //ntrip 2.0
                client.println("HTTP/1.1 200 OK");
              }
              if(client_status &(1 << 5)){
                client.println("HTTP/1.1 401");
                client.stop();
                return 2;
                //user incorreto
              }
              if(client_status &(1 << 6)){
                client.println("HTTP/1.1 401");
                client.stop();
                return 3;
                //pass incorreto
              }
              if(client_status &(1 << 3)){
                client.println("HTTP/1.1 404");
                client.stop();
                return 4;
                //mntp incorreto   
              }
              Serial.println("tudo ok!");
              while(client.connected()){
                if(serialPort.available()) {  
                  char ch_ap[1024];
                  int readcount_ap = 0;
                  Serial.println(serialPort.available());
                  while (serialPort.available()) {
                    ch_ap[readcount_ap] = serialPort.read();
                    readcount_ap++;
                    if (readcount_ap > 511)break;
                    Serial.println("while4");
                  }//buffering
                  
                  client.write((uint8_t*)ch_ap, readcount_ap);
                  Serial.println(readcount_ap);
                }
                if (client.available()!=0) {
                  break;
                }
                //Serial.println(client.available());
                Serial.print("client Connected: ");
                Serial.println(client.connected());
                Serial.print("client Available: ");
                Serial.println(client.available());
                Serial.println("while3");
              }
              return 1;
            }
          }else{
            //trata erro!!! nao http
            client.println("HTTP/1.1 400");
            client.stop();
            return 5;
          }
        }else{
          //client.println("HTTP/1.1 502");
          client.stop();
          return 0;
        }
      }
      Serial.println("while2");
    }
  }
}

/*int NTRIPLocal(WiFiClient& client, Stream& serialPort, char* User, char* Pass, char* Mountpoint){
  static int counter = 0; 
  while (client.available()>0){ 
    char client_data[1024]; 
    client_data[counter] = client.read();
    counter++;
    if(client.available()==0){
      String client_data_s = String(client_data);
      Serial.println(client_data_s.indexOf("GET"));
      Serial.println(client_data_s);
      memset(client_data,' ',sizeof(client_data)*sizeof(char));
      if(client_data_s.indexOf("GET")==0){
        
        int client_status = ntrip.analize_data(client_data_s,User,Pass,Mountpoint);
        if(client_status!=0){
          if(client_status &(1 << 4)){
            //solicitando mntp
            client.println("SOURCETABLE 200 OK");
            client.println(ntrip.scrtbl("ntripteste",WiFi.softAPIP().toString(),2101));
            client.stop();
            return 6;
          }
          Serial.println(client_status); 
          if(client_status &(1 << 1)||client_status &(1 << 2)){
            // tudo ok!
            if(client_status &(1 << 1)){
              //ntrip 1.0
              client.println("ICY 200 OK");
            }
            if(client_status &(1 << 2)){
              //ntrip 2.0
              client.println("HTTP/1.1 200 OK");
            }
            if(client_status &(1 << 5)){
              client.println("HTTP/1.1 401");
              client.stop();
              return 2;
              //user incorreto
            }
            if(client_status &(1 << 6)){
              client.println("HTTP/1.1 401");
              client.stop();
              return 3;
              //pass incorreto
            }
            if(client_status &(1 << 3)){
              client.println("HTTP/1.1 404");
              client.stop();
              return 4;
              //mntp incorreto   
            }
            Serial.println("tudo ok!");
          }
        }else{
          //trata erro!!! nao http
          client.println("HTTP/1.1 400");
          client.stop();
          return 5;
        }
      }else{
        //client.println("HTTP/1.1 502");
        client.stop();
        return 0;
      }
    }
    Serial.println("while2");
  }
  while(client.connected()){
    if(serialPort.available()) {  
      char ch_ap[1024];
      int readcount_ap = 0;
      Serial.println(serialPort.available());
      while (serialPort.available()) {
        ch_ap[readcount_ap] = serialPort.read();
        readcount_ap++;
        if (readcount_ap > 511)break;
        Serial.println("while4");
      }//buffering
      
      client.write((uint8_t*)ch_ap, readcount_ap);
      Serial.println(readcount_ap);
    }
    if (client.available()!=0) {
      break;
    }
    //Serial.println(client.available());
    Serial.print("client Connected: ");
    Serial.println(client.connected());
    Serial.print("client Available: ");
    Serial.println(client.available());
    Serial.println("while3");
  } 
  return 1;
} */

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
  ntrip.NTRIPLocalSetup("testentrip", NULL);
  ntripServer.begin();
  Serial1.println("AAAAAAAA");
}

void loop() {
  millisfunc();
  WiFiClient client = ntripServer.available();
  if(client){
    NTRIPLocal(client, Serial, "user", "passw", "ntripteste");
  }
  /*if (client.available()>0) {
    if(Serial.available()) {  
      char ch_ap[1024];
      int readcount_ap = 0;
      Serial.println(Serial.available());
      while (Serial.available()) {
        ch_ap[readcount_ap] = Serial.read();
        readcount_ap++;
        if (readcount_ap > 511)break;
        Serial.println("while4");
      }//buffering
      
      client.write((uint8_t*)ch_ap, readcount_ap);
      Serial.println(readcount_ap);
    }
    //if (client.available()!=0) {
    //  setup = 0;
    //}
    //Serial.println(client.available());
    Serial.print("client Connected: ");
    Serial.println(client.connected());
    Serial.print("client Available: ");
    Serial.println(client.available());
    Serial.println("while3");
  } */
}

void millisfunc() {
  static int lastmillis = 0;
  int millisatual = millis();
  if (millisatual - lastmillis > 1000) {
    Serial.println("while out");
    lastmillis = millisatual;
  }
}
