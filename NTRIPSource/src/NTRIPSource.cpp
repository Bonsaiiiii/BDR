#include "NTRIPSource.h"
#include "arduino_base64.hpp"

int NTRIPSource::subStation(WiFiClient& client, int port, char* host, char* mntpnt, String rev, char* user, char* psw, char* info)
{
  if(!client.connect(host,port)){
    //Serial.println("host/port incorreto");
    return 1;
  } 
  char buff[256 + NTRIP_MAXSTR], *p = buff;
  char* response;
  
  if (rev == "N") {
    p += sprintf(p, "SOURCE %s %s\r\n", psw, mntpnt);
    response = "ICY 200 OK";
  } 
  if (rev == "S") {
    String credentials = String(user) + ":" + String(psw);
    const char* credentialsStr = credentials.c_str();
    size_t credentialsLen = strlen(credentialsStr);
    char authEncoded[1024];
    base64::encode((const uint8_t*)credentialsStr, credentialsLen, authEncoded);

    //p += sprintf(p, "SOURCE %s HTTP/1.1\r\n", mntpnt);
    //p += sprintf(p, "SOURCE /%s NTRIP/2.0\r\n", mntpnt);
    p += sprintf(p, "POST /%s HTTP/1.1\r\n", mntpnt);
    p += sprintf(p, "Host: %s\r\n", host);
    p += sprintf(p, "Authorization: Basic %s\r\n", authEncoded);
    p += sprintf(p, "Ntrip-Version: Ntrip/2.0\r\n");
    response = "HTTP/1.1 200 OK";
  }
  p += sprintf(p, "Source-Agent: BDR>01 version 1.5\r\n");
  p += sprintf(p, "STR: %s\r\n", info);
  p += sprintf(p, "\r\n");

  client.print(buff);
  #ifdef Debug
  //Serial.println(p);
  #endif
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 20000) {
          //Serial.println("Client Timeout !");
          return 4;
      }
      delay(10);
  }
  delay(50);

  int buffer_size = client.available();
  char buffer[buffer_size + 1];
  client.readBytes(buffer, buffer_size);

  //char buffer[50];
  client.read((uint8_t*)buffer,sizeof(buffer));
  Serial.print("buffer= ");
  Serial.print((char*)buffer);
  if(strncmp((char*)buffer,"HTTP/1.1 404",12)==0||strncmp((char*)buffer,"404 Not found",13)==0||strncmp((char*)buffer,"ICY 404",7)==0){
    //Serial.println("não encontrado!");
    return 5;
  }
  /*
  if(strncmp((char*)buffer,"HTTP/1.1 403",12)==0||strncmp((char*)buffer,"403 Forbidden",13)==0){
      //Serial.println("autentificação incoreta!");
      return 7;
  }
  */
  if(strncmp((char*)buffer,"HTTP/1.1 401",12)==0||strncmp((char*)buffer,"401 Unauthorized",16)==0||strncmp((char*)buffer,"ICY 401 Unauthorized",20)==0){
      //Serial.println("autentificação incoreta!");
      return 3;
  }
  if(strncmp((char*)buffer,"HTTP/1.1 200 OK",15)==0||strncmp((char*)buffer,"200 OK",6)==0||strncmp((char*)buffer,"ICY 200 OK",10)==0)
  {   
    //Serial.println("teste ok");
    return 0;
  }
  //Serial.println("mucho loco");
  return 2; // resposta incompreendida
  /*if(strncmp((char*)buffer,response,10))
  {
    Serial.print((char*)buffer);
    return false;
  } */
  //return true;
} // BBBBBBBBBBBBB


//int sourceError;

int NTRIPSource::NTRIPSourceSetup(char* Network, char* Password) {
  WiFi.begin(Network, Password);
  static int i = 0;
  while (WiFi.status() != WL_CONNECTED&&i<50) {
    Serial.print(".");
    i++;
    delay(200);
  }
  return true;
}

//int NTRIPSource::getSourceError() {
//  return sourceError;
//}

/*int NTRIPSource::NTRIPSourceStart(Stream& serialPort, WiFiClient& client, int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int rev, char* NTPUser, char* NTPPass, char* srcSTR) {

  //if (setupResult != 1) {
  //  Serial.println("Setup failed. Exiting...");
  //  return -1;  // Early exit if setup fails
  //}
  static int setupResult;
  static bool substationExecuted = false;
  if (!substationExecuted) {
    NTRIPSourceSetup(Network, Password);
    Serial.println("Subscribing MountPoint is OK");
    sourceError = subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR);
    if (sourceError == 4 || sourceError == 2) {
      setupResult = 1;
      Serial.println("setup success");
    } else if (sourceError == 1 || sourceError == 6 || sourceError == 5 || sourceError == 4) {
      setupResult = 0;
      Serial.println("substation fail");
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
      Serial.print("readcount: ");Serial.println(readcount);
      client.write((uint8_t*)ch, readcount);
      readcount = 0;
      memset(ch,' ',readcount*sizeof(char));
    }
  }
  else {
    client.stop();
    //Serial.println("reconnect");
    //Serial.println("Subscribing MountPoint");
    static int t = 0;
    if (subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR) != 4) {
      t++;
      if (t > 10) {
        t = 0;
        substationExecuted = false;
      }
      delay(100);
    }
    else {
      //Serial.println("Subscribing MountPoint is OK");
      delay(10);
    }

  }
  delay(10);  //server cycle
  return true;
}  */

 // String NTRIPSource::makeOptionalSTR(char* dataFormat, char* details, char* carrier, char* navSys, char* network, country,char* lat,char*lon,char* nmea,char sol,char gen,comEnc,char* auth,fee,bitrate);
