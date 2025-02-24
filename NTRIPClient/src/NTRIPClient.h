#ifndef NTRIP_CLIENT
#define NTRIP_CLIENT

#include <WiFiClient.h>
#include <Arduino.h>
#include<base64.h>

class NTRIPClient : public WiFiClient{
  public :
  int reqSrcTbl(char* host,int &port, String id);   //request MountPoints List serviced the NTRIP Caster 
  int reqRaw(char* host,int &port,char* mntpnt,char* user,char* psw, String gga, String check, String id);   //with gga   //request RAW data from Caster 
  int reqRaw(char* host,int &port,char* mntpnt,char* user,char* psw, String id); //with user
  int reqRaw(char* host,int &port,char* mntpnt, String id); //non user
  int sendgga(String gga);
  int readLine(char* buffer,int size);

  
};

#endif
