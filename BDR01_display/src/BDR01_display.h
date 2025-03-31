#ifndef BDR01_DISPLAY_H
#define BDR01_DISPLAY_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Scroller.h"

class BDRDisplay {
public:
    BDRDisplay();
    ~BDRDisplay();
    void initDisplay();
    void scrollerInitClientSource(char* Wrede, const char* Wip, char* Nrede);
    void scrollerInitLocal(char* Wssid, char* Wpass, char* Nhost, char* Nmtpt, char* Nuser, char* Npass);
    void page_client(int page, char* statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* Wrede, const char* Wip, char* Nrede, int rssi, String Nlaten, char* uptim,
                     String locat, char* lat, char* lon, char* alt, char* pre, char* utc, char* tensao, String pacot, String Slaten, char* modelo, 
                     char* firmware, String serial);
    void page_local(int page, char* statusOUT, int NStatus, int Ssaida, int PDvolts, char* Wssid, char* Wpass, char* Nhost, char* Nmtpt, char* Nuser, char* Npass,
        String Wusers, char* Nport, char* baud, char* pari, char* data, char* stop, char* flow, char* tensao, String pacot, String Slaten, char* modelo, char* firmware,
        String serial);
    void page_source(int page, char* statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* Wrede, const char* Wip, char* Nrede, int rssi, String Nlaten, char* uptim,
                     char* tensao, String pacot, String Slaten, char* modelo, char* firmware, String serial);
    Scroller* scrollers[6];
    Adafruit_SSD1306 display;
    void display_fixo(int modo, int page, String statusOUT);
    void texto_fixo_inferior(int state);
    void page_client_source(int page, int Swifi, int Sntrip, int Ssaida, int rssi, String Nlaten, char* uptim);
    void page_only_local(int page, int NStatus, String Wusers, char* Nport, char* baud, char* pari, char* data, char* stop, char* flow);
    void tela_saida(char* tensao, int Ssaida, String pacot, String Slaten);
    void tela_entrar_config();
    void tela_localizacao(char* lat, char* lon, char* alt, char* pre, char* utc, String locat);
    void tela_info_alimentacao(int PDvolts);
    void tela_info_bdr(char* modelo, char* firmware, String serial); 
    void display_config(int page, bool conectado, int statusPD, int alimentacao, char* modelo, char* firmware, String serial);
private:
};

#endif
