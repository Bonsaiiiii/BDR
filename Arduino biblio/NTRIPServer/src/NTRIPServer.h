#ifndef NTRIPServer_h
#define NTRIPServer_h

#include <WiFi.h>

class NTRIPServer {
public:
    NTRIPServer(uint16_t port = 2101);
    void begin(const char* mountpoint, const char* user, const char* password, 
               const char* ssid, const char* apPassword = NULL, 
               IPAddress ip = IPAddress(192, 168, 0, 1), 
               IPAddress gateway = IPAddress(192, 168, 0, 1), 
               IPAddress subnet = IPAddress(255, 255, 255, 0));
    void handle();

private:
    String generateSourceTable(const char* mountpoint, String ip, uint16_t port);
    int checkAuthentication(String data, String user, String pwr);
    int checkHTTPVersion(String data);
    int checkMountpoint(String data);

    char cliente_data_ap[1024];
    char ch_ap[1024];
    WiFiServer _ntripServer;
    WiFiClient _client;
    unsigned long currentMillis_ap;
    unsigned long atualMillis_ap;
    bool flag_ok_ap;
    bool flag_send_ap;
    unsigned long int readcount_ap;

    String _mountpoint;
    String _user;
    String _password;
    const char* _ssid;
    const char* _apPassword;
    IPAddress _ip;
    IPAddress _gateway;
    IPAddress _subnet;
    uint16_t _port;
};

#endif