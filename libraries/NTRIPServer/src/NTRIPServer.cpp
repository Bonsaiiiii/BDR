#include "NTRIPServer.h"
#include "base64.hpp"
#include "HardwareSerial.h"

void NTRIPServer::NTRIPLocalSetup(const char* Network, const char* Password,
    IPAddress ip, IPAddress gateway, IPAddress subnet) {
_Network = Network;
_Password = Password;
_ip = ip;
_gateway = gateway;
_subnet = subnet;

WiFi.softAPConfig(_gateway, _ip, _subnet);
delay(100);
WiFi.mode(WIFI_AP);
WiFi.softAP(Network, Password, 7, 0, 10);
while (WiFi.softAPIP() == INADDR_NONE) {
  delay(500);
  Serial.print(".");
}
}

String NTRIPServer::scrtbl(const char* mountpoint, String ip, uint16_t port){
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
  

int NTRIPServer::analize_data(String data, String user, String password, String mountpoint){
    int state = 0;
    char base64[50];
    unsigned char string[50];
    int posicao_http = data.indexOf("HTTP");
    int posicao_rn = data.indexOf("\r\n");
    int posicao_bara = data.indexOf("/");
    int posicao_basic = data.indexOf(": Basic") + 8;
    if (posicao_http < 0){
      Serial.println("ERRO1");
      return 0; //ERRO1 
    }
    String versao_http = data.substring(posicao_http, posicao_rn);
    if(versao_http == "HTTP/1.0"){
      Serial.println("ERRO2");
      state |= 1 << 1;
    } else {
      state |= 0 << 1;
    }
    if(versao_http == "HTTP/1.1"){
      Serial.println("ERRO3");
      state |= 1 << 2;
    } else {
      state |= 0 << 2;
    }
    String cliente_mountpoint = data.substring(posicao_bara,posicao_http-1);
    if(cliente_mountpoint != (String("/") + mountpoint)){
      Serial.println("ERRO4");
      state |= 1 << 3; 
    } else if (cliente_mountpoint == (String("/") + mountpoint)) {
      state |= 0 << 3;
    }
    if(cliente_mountpoint == "/SOURCETABLE.TXT " || cliente_mountpoint == "/"){
      Serial.println("ERRO5");
      state |= 1 << 4;
    } else {
      state |= 0 << 4;
    }
    String usuario_encoded = data.substring(posicao_basic);
    usuario_encoded.toCharArray(base64,50);
    unsigned int string_length = decode_base64((unsigned char *)base64, string);
    string[string_length] = '\0';
    String usuario_decoded = String((char *)string);
    int separador = usuario_decoded.indexOf(":");
    if(usuario_decoded.substring(0,separador) != user){
      Serial.println("ERRO6");
      state |= 1 << 5;
    } else if (usuario_decoded.substring(0,separador) == user) {
      state |= 0 << 5;
    }
    if(usuario_decoded.substring(separador+1) != password){
      Serial.println("ERRO7");
      state |= 1 << 6;
    } else if (usuario_decoded.substring(separador+1) == password) {
      state |= 0 << 6;
    }
    return state;
}

/*
int NTRIPServer::NTRIPLocal(WiFiClient& client, Stream& serialPort, char* User, char* Pass, char* Mountpoint){
  char client_data[1024];
  while(client.connected()){
    int counter = 0; 
    Serial.println("while1");
    while (client.available()>0){  
      client_data[counter] = client.read();
      counter++;
      if(client.available()==0){
        String client_data_s = String(client_data);
        Serial.println(client_data_s.indexOf("GET"));
        Serial.println(client_data);
        memset(client_data,' ',sizeof(client_data)*sizeof(char));
        if(client_data_s.indexOf("GET")==0){
          
          int client_status = analize_data(client_data_s,User,Pass,Mountpoint);
          if(client_status!=0){
            if(client_status &(1 << 4)){
              //solicitando mntp
              client.println("SOURCETABLE 200 OK");
              client.println(scrtbl("ntripteste",WiFi.softAPIP().toString(),2101));
              client.stop();
              return 6;
            }
            if(client_status &(1 << 3)&&client_status &(1 << 5)&&client_status &(1<<6)){
              // tudo ok!
              if(client_status &(1 << 1)){
                //ntrip 1.0
                client.println("ICY 200 OK");
              }
              if(client_status &(1 << 2)){
                //ntrip 2.0
                client.println("HTTP/1.1 200 OK");
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
                  //Serial.println(readcount_ap);
                } 
                Serial.println("while3");
              }
              return 1;
            }else{
              if(client_status &(0 << 5)){
                client.println("HTTP/1.1 401");
                client.stop();
                return 2;
                //user incorreto
              }
              if(client_status &(0 << 6)){
                client.println("HTTP/1.1 401");
                client.stop();
                return 3;
                //pass incorreto
              }
              if(client_status &(0 << 3)){
                client.println("HTTP/1.1 404");
                client.stop();
                return 4;
                //mntp incorreto   
              } 
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
} */