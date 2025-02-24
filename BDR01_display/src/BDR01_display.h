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
    void scrollerInitClientSource(char* Wrede, char* Wip, char* Nrede);
    void scrollerInitLocal(char* Wssid, char* Wpass, char* Nhost, char* Nmtpt, char* Nuser, char* Npass);
    void page_client(int page, int statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* Wrede, char* Wip, char* Nrede, char* rssi, char* Nlaten, char* uptim,
                     char* lat, char* lon, char* alt, char* pre, char* utc, char* tensao, char* pacot, char* Slaten, char* modelo, 
                     char* firmware, char* serial);
    void page_local(int page, int statusOUT, int Ssaida, int PDvolts, char* Wssid, char* Wpass, char* Nhost, char* Nmtpt, char* Nuser, char* Npass,
        char* Wusers, char* Nport, char* baud, char* pari, char* data, char* stop, char* flow, char* tensao, char* pacot, char* Slaten, char* modelo, char* firmware,
        char* serial);
    void page_source(int page, int statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* Wrede, char* Wip, char* Nrede, char* rssi, char* Nlaten, char* uptim,
                     char* tensao, char* pacot, char* Slaten, char* modelo, char* firmware, char* serial);
    Scroller* scrollers[6];
    Adafruit_SSD1306 display;
    void display_fixo(int modo, int page, int statusOUT);
    void texto_fixo_inferior(int state);
    void page_client_source(int page, int Swifi, int Sntrip, int Ssaida, char* rssi, char* Nlaten, char* uptim);
    void page_only_local(int page, char* Wusers, char* Nport, char* baud, char* pari, char* data, char* stop, char* flow);
    void tela_saida(char* tensao, int Ssaida, char* pacot, char* Slaten);
    void tela_entrar_config();
    void tela_localizacao(char* lat, char* lon, char* alt, char* pre, char* utc);
    void tela_info_alimentacao(int PDvolts);
    void tela_info_bdr(char* modelo, char* firmware, char* serial); 
    void display_config(int page, bool conectado, int statusPD, char* modelo, char* firmware, char* serial);
private:
};

#endif
