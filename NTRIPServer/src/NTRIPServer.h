#ifndef NTRIPServer_h
#define NTRIPServer_h

#include <WiFi.h>

class NTRIPServer {
public:
    void NTRIPLocalSetup(const char* Network, const char* Password,
               IPAddress ip = IPAddress(192, 168, 0, 1), 
               IPAddress gateway = IPAddress(192, 168, 0, 1), 
               IPAddress subnet = IPAddress(255, 255, 255, 0));
    int NTRIPLocal(WiFiClient& client, Stream& serialPort);
    void handle();

private:
    String scrtbl(const char* mountpoint, String ip, uint16_t port);
    int analize_data(String data, String user, String password, String mountpoint);

    char client_data[1024];
    char ch_ap[1024];
    unsigned long int readcount_ap;

    const char* _Network;
    const char* _Password;
    IPAddress _ip;
    IPAddress _gateway;
    IPAddress _subnet;
};

#endif