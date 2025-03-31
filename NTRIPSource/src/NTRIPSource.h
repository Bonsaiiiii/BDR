#ifndef NTRIPSOURCE_H_
#define NTRIPSOURCE_H_

#include <Arduino.h>
#include <WiFi.h>

#define NTRIP_MAXSTR 256

class NTRIPSource {
public:
    //int sourceError;
    //int getSourceError();
    int NTRIPSourceSetup(char* Network, char* Password);
    //int NTRIPSourceStart(Stream& serialPort, WiFiClient& client, int HTTPPort, char* Network, char* Password, char* Host, char* MountPoint, int rev, char* NTPUser, char* NTPPass, char* srcSTR); 
    int subStation(WiFiClient& client, int port, char* host, char* mntpnt, String rev, char* user, char* psw, char* info); 
};

#endif // NTRIPSOURCE_H_