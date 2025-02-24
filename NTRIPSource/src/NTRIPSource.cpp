#include "NTRIPSource.h"
#include "arduino_base64.hpp"

bool NTRIPSource::subStation(WiFiClient& client, int &port, char* host, char* mntpnt, int rev, char* user, char* psw, char* info)
{
    if(!client.connect(host,port))return false;
  char buff[256 + NTRIP_MAXSTR], *p = buff;
  char* response;
  
  if (rev == 1) {
    p += sprintf(p, "SOURCE %s %s\r\n", psw, mntpnt);
    response = "ICY 200 OK";
  } 
  if (rev == 2) {
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
  Serial.println(p);
  #endif
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 20000) {
          Serial.println("Client Timeout !");
          return false;
      }
      delay(10);
  }
  char buffer[50];
  client.read((uint8_t*)buffer,sizeof(buffer));
  if(strncmp((char*)buffer,response,10))
  {
    Serial.print((char*)buffer);
    return false;
  }
  return true;
}

int NTRIPSource::NTRIPSourceSetup(WiFiClient& client, int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int rev, char* NTPUser, char* NTPPass, char* srcSTR) {
  WiFi.begin(Network, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Subscribing MountPoint");
  if (!subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR)) {
    delay(15000);
    ESP.restart();
  }
  Serial.println("Subscribing MountPoint is OK");
  return true;
}

int NTRIPSource::NTRIPSourceStart(Stream& serialPort, WiFiClient& client, int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int rev, char* NTPUser, char* NTPPass, char* srcSTR) {
  static bool substationExecuted = false;
  if (!substationExecuted) {
    NTRIPSourceSetup(client, HTTPPort, Network, Password, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR);
    Serial.println("Subscribing MountPoint is OK");
    substationExecuted = true;
  }
  char ch[512];
  int readcount = 0;// put your main code here, to run repeatedly:
  if (client.connected()) {
    while (serialPort.available()) {
      
      while (serialPort.available()) {
        ch[readcount] = serialPort.read();
        readcount++;
        if (readcount > 511)break;
        //Serial.println(Serial2.read());
      }//buffering
      //Serial.print("readcount: ");Serial.println(readcount);
      client.write((uint8_t*)ch, readcount);
      readcount = 0;
      memset(ch,' ',readcount*sizeof(char));
    }
  }
  else {
    client.stop();
    Serial.println("reconnect");
    Serial.println("Subscribing MountPoint");
    if (!subStation(client, HTTPPort, Host, MountPoint, rev, NTPUser, NTPPass, srcSTR)) {
      delay(100);
    }
    else {
      Serial.println("Subscribing MountPoint is OK");
      delay(10);
    }

  }
  delay(10);  //server cycle
  return true;
}

 // String NTRIPSource::makeOptionalSTR(char* dataFormat, char* details, char* carrier, char* navSys, char* network, country,char* lat,char*lon,char* nmea,char sol,char gen,comEnc,char* auth,fee,bitrate);
