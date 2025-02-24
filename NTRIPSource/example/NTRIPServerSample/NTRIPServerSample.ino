#include <WiFi.h>
#include<HardwareSerial.h>
#include <NTRIPSource.h>

NTRIPSource ntripS;
WiFiClient ntrip_s;

/*int NTRIPSourceSetup(int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int REV2, char* NTPUser, char* NTPPass, char* srcSTR) {
  WiFi.begin(Network, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println("Subscribing MountPoint");
  if (!ntrip_s.subStation(HTTPPort, Host, MountPoint, REV2, NTPUser, NTPPass, srcSTR)) {
    delay(15000);
    ESP.restart();
  }
  Serial.println("Subscribing MountPoint is OK");
  return true;
}

int NTRIPSource(int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int REV2, char* NTPUser, char* NTPPass, char* srcSTR) {
  char ch[512];
  int readcount = 0;// put your main code here, to run repeatedly:
  if (ntrip_s.connected()) {
    while (Serial2.available()) {
      
      while (Serial2.available()) {
        ch[readcount] = Serial2.read();
        readcount++;
        if (readcount > 511)break;
        //Serial.println(Serial2.read());
      }//buffering
      //Serial.print("readcount: ");Serial.println(readcount);
      ntrip_s.write((uint8_t*)ch, readcount);
      readcount = 0;
      memset(ch,' ',readcount*sizeof(char));
    }
  }
  else {
    ntrip_s.stop();
    Serial.println("reconnect");
    Serial.println("Subscribing MountPoint");
    if (!ntrip_s.subStation(HTTPPort, Host, MountPoint, REV2, NTPUser, NTPPass, srcSTR)) {
      delay(100);
    }
    else {
      Serial.println("Subscribing MountPoint is OK");
      delay(10);
    }

  }
  delay(10);  //server cycle
  return true;
} */

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1, 44, 43);
  //Serial2.begin(115200, SERIAL_8N1, 17, 16);
  delay(10);
  //Serial.println("exit setup");
}

void loop() {
  ntripS.NTRIPSourceStart(Serial2, ntrip_s, 2101, "HugenPLUS", "H32851112", "192.168.15.16", "juca", 2, "jucas", "jucapass", "");
}
