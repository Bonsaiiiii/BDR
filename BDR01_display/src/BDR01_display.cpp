#include "BDR01_display.h"
#include "Adafruit_SSD1306.h"
#include "Scroller.h"

#define DEBUG 1
#define CF3 4
#define CF2 5
#define CF1 6
#define PDPG 7
#define LED2B 15
#define LED2R 16
#define LED2G 17
#define ENTER 3
#define PAGE 13
#define POUT 9
#define SDA 10
#define SCL 11
#define DRST 12
#define DPWR 14
#define DET 21
#define TX 2
#define RX 1
#define LED1B 43
#define LED1R 44
#define LED1G 42
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DSP_ADDR 0x3c
#define OLED_RESET     DRST
#define SCREEN_ADDRESS DSP_ADDR

void BDRDisplay::initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Halt on failure
  }
  display.clearDisplay();
}

BDRDisplay::BDRDisplay() 
  : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) 
{
  for (auto& scroller : scrollers) {
    scroller = nullptr;
  }
}

BDRDisplay::~BDRDisplay() {
  for (auto& scroller : scrollers) {
    delete scroller;
    scroller = nullptr;
  }
}

void BDRDisplay::scrollerInitClientSource(char* Wrede, char* Wip, char* Nrede) {
  scrollers[0] = new Scroller(30, 128, 22, Wrede, display);
  scrollers[1] = new Scroller(18, 74, 44, Wip, display);
  scrollers[2] = new Scroller(30, 128, 22, Nrede, display);
}

void BDRDisplay::scrollerInitLocal(char* Wssid, char* Wpass, char* Nhost, char* Nmtpt, char* Nuser, char* Npass) {
  scrollers[0] = new Scroller(30, 128, 22, Wssid, display);
  scrollers[1] = new Scroller(30, 128, 33, Wpass, display);
  scrollers[2] = new Scroller(30, 128, 22, Nhost, display);
  scrollers[3] = new Scroller(30, 128, 33, Nmtpt, display);
  scrollers[4] = new Scroller(30, 128, 22, Nuser, display);
  scrollers[5] = new Scroller(30, 128, 33, Npass, display);
}

void BDRDisplay::page_client(int page, int statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* rssi, char* Nlaten, char* uptim, 
                char* lat, char* lon, char* alt, char* pre, char* utc, char* tensao, char* pacot, char* Slaten, 
                char* modelo, char* firmware, char* serial) {
  display_fixo(1, page, statusOUT);
  switch(page) {
    case 0:
      page_client_source(0, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
      break;
    case 1: 
      page_client_source(1, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
      break;
    case 2:
      page_client_source(2, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
      break;
    case 3:
      tela_localizacao(lat, lon, alt, pre, utc);
      break;
    case 4:
      tela_saida(tensao, Ssaida, pacot, Slaten);
      break;
    case 5:
      tela_entrar_config();
      break;
    case 6:
      tela_info_alimentacao(PDvolts);
      break;
    case 7: 
      tela_info_bdr(modelo, firmware, serial);
      break;
  }
}

void BDRDisplay::page_local(int page, int statusOUT, int Ssaida, int PDvolts, char* Wusers, char* Nport, char* baud, char* pari, char* data, char* stop,
char* flow, char* tensao, char* pacot, char* Slaten, char* modelo, char* firmware, char* serial) {
  display_fixo(2, page, statusOUT);
  switch(page) {
    case 0:
      page_only_local(0, Wusers, Nport, baud, pari, data, stop, flow);
    break;
    case 1:
      page_only_local(1, Wusers, Nport, baud, pari, data, stop, flow);
    break;
    case 2:
      page_only_local(2, Wusers, Nport, baud, pari, data, stop, flow);
    break;
    case 3:
      page_only_local(3, Wusers, Nport, baud, pari, data, stop, flow);
    break;
    case 4:
      tela_saida(tensao, Ssaida, pacot, Slaten);
    break;
    case 5:
      tela_entrar_config();
    break;
    case 6:
      tela_info_alimentacao(PDvolts);
    break;
    case 7:
      tela_info_bdr(modelo, firmware, serial);
    break;
  }
}

void BDRDisplay::page_source(int page, int statusOUT, int Swifi, int Sntrip, int Ssaida, int PDvolts, char* rssi, char* Nlaten, char* uptim,
char* tensao, char* pacot, char* Slaten, char* modelo, char* firmware, char* serial) {
  display_fixo(3, page, statusOUT);
  switch(page) {
    case 0:
      page_client_source(0, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
    break;
    case 1: 
      page_client_source(1, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
    break;
    case 2:
      page_client_source(2, Swifi, Sntrip, Ssaida, rssi, Nlaten, uptim);
    break;
    case 3:
      tela_saida(tensao, Ssaida, pacot, Slaten);
    break;
    case 4:
      tela_entrar_config();
    break;
    case 5:
      tela_info_alimentacao(PDvolts);
    break;
    case 6:
      tela_info_bdr(modelo, firmware, serial);
    break;
  }
}

void BDRDisplay::texto_fixo_inferior(int state) {
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.fillRect(0, 54, 128, 64, 0);
  display.drawLine(0, 55, 128, 55, 1);
  switch(state){
    case 0: 
      display.setCursor(0, 57);
      display.print("PAGINA");
      display.setCursor(98, 57);
      display.print("ENTER");
    break;
    case 1:
      display.setCursor(0, 57);
      display.print("NAO");
      display.setCursor(110, 57);
      display.print("SIM");
    break;
  }
}

void BDRDisplay::page_client_source(int page, int Swifi, int Sntrip, int Ssaida, char* rssi, char* Nlaten, char* uptim) {
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  switch(page) {
    case 0: 
      display.fillRect(0, 10, 128, 45, 0); 
      display.setCursor(32, 11);
      display.print("STATUS BDR");
      display.setCursor(0, 22);
      display.print("WIFI:");
        if (Swifi == 0) {
          display.print("BUSCANDO...");
        } else if (Swifi == 1) {
          display.print("CONECTANDO...");
        } else if (Swifi == 2) {
          display.print("DESCONECTADO");
        } else if (Swifi == 3) {
          display.print("NAO ENCONTRADA");
        } else if (Swifi == 4) {
          display.print("SENHA INCORRETA");
        } else if (Swifi == 5) {
          display.print("CONECTADO");
        }
      display.setCursor(0, 33);
      display.print("NTRIP:");
        if (Sntrip == 0) {
          display.print("MTPT INCORRETO");
        } else if (Sntrip == 1) {
          display.print("USR/SEN INCORRETA");
        } else if (Sntrip == 2) {
          display.print("HOST INVALIDO");
        } else if (Sntrip == 3) {
          display.print("CONECTANDO...");
        } else if (Sntrip == 4 || Sntrip == 5) {
          display.print("CONECTADO");
        }
      display.setCursor(0, 44);
      display.print("SAIDA:");
      if (Ssaida == 0) {
        display.print("DESCONECTADO");
      } else if (Ssaida == 1) {
        display.print("CONECTADO");
      }
    break;
    case 1:
      display.fillRect(0, 11, 128, 11, 0); 
      display.setCursor(30, 11);
      display.print("WIFI STATUS");
      
      scrollers[0]->scrollLine();
      display.fillRect(0, 22, 30, 11, 0);
      display.setCursor(0, 22);
      display.print("REDE:");

      display.fillRect(0, 33, 128, 11, 0);
      display.setCursor(0, 33);
      display.print("STATUS:");
      if (Swifi == 0) {
          display.print("BUSCANDO...");
        } else if (Swifi == 1) {
          display.print("CONECTANDO...");
        } else if (Swifi == 2) {
          display.print("DESCONECTADO");
        } else if (Swifi == 3) {
          display.print("NAO ENCONTRADA");
        } else if (Swifi == 4) {
          display.print("SENHA INCORRETA");
        } else if (Swifi == 5) {
          display.print("CONECTADO");
        }
      
      scrollers[1]->scrollLine();
      display.fillRect(0, 44, 18, 11, 0);
      display.setCursor(0, 44);
      display.print("IP:");
      display.fillRect(73, 44, 55, 11, 0);
      display.setCursor(74, 44);
      display.print("RSSI:");
      display.print(rssi);
    break;
    case 2:
      display.fillRect(0, 11, 128, 11, 0);
      display.fillRect(0, 33, 128, 11, 0);
      display.fillRect(0, 44, 128, 11, 0); 
      display.setCursor(30, 11);
      display.print("NTRIP STATUS");

      scrollers[2]->scrollLine();
      display.fillRect(0, 22, 30, 11, 0);
      display.setCursor(0, 22);
      display.print("REDE:");

      display.setCursor(0, 33);
      display.print("STATUS:");
        if (Sntrip == 0) {
          display.print("MTPT INCORRETO");
        } else if (Sntrip == 1) {
          display.print("USR/SEN INCORRETA");
        } else if (Sntrip == 2) {
          display.print("HOST INVALIDO");
        } else if (Sntrip == 3) {
          display.print("CONECTANDO...");
        } else if (Sntrip == 4 || Sntrip == 5) {
          display.print("CONECTADO");
        }

      display.setCursor(0, 44);
      display.print("LATEN:");
      display.print(Nlaten);
      display.setCursor(63, 44);
      display.print("UPTIM:");
      display.print(uptim);
    break;
  }
}

void BDRDisplay::page_only_local(int page, char* Wusers, char* Nport, char* baud, char* pari, char* data, char* stop, char* flow) {
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  switch(page) {
    case 0:
      display.fillRect(0, 11, 128, 11, 0); 
      display.setCursor(35, 11);
      display.print("WIFI INFO");
  
      scrollers[0]->scrollLine();
      display.fillRect(0, 22, 30, 11, 0);
      display.setCursor(0, 22);
      display.print("SSID:");

      scrollers[1]->scrollLine();
      display.fillRect(0, 33, 30, 11, 0);
      display.setCursor(0, 33);
      display.print("PASS:");

      display.fillRect(0, 44, 128, 11, 0); 
      display.setCursor(0, 44);
      display.print("USERS:");
      display.print(Wusers);
    break;
    case 1:
      display.fillRect(0, 11, 128, 11, 0); 
      display.setCursor(32, 11);
      display.print("NTRIP INFO");

      scrollers[2]->scrollLine();
      display.fillRect(0, 22, 30, 11, 0);
      display.setCursor(0, 22);
      display.print("HOST:");

      scrollers[3]->scrollLine();
      display.fillRect(0, 33, 30, 11, 0);
      display.setCursor(0, 33);
      display.print("MTPT:");

      display.fillRect(0, 44, 128, 11, 0);
      display.setCursor(0, 44);
      display.print("PORT:");
      display.print(Nport);
    break;
    case 2:
      display.fillRect(0, 11, 128, 11, 0); 
      display.setCursor(32, 11);
      display.print("NTRIP INFO");

      scrollers[4]->scrollLine();
      display.fillRect(0, 22, 30, 11, 0);
      display.setCursor(0, 22);
      display.print("USER:");

      scrollers[5]->scrollLine();
      display.fillRect(0, 33, 30, 11, 0);
      display.setCursor(0, 33);
      display.print("PASS:");
    break;
    case 3:
      display.fillRect(0, 10, 128, 45, 0); 
      display.setCursor(30, 11);
      display.print("SERIAL PORT");
      display.setCursor(0, 22);
      display.print("BAUD:");
      display.print(baud);
      display.setCursor(80, 22);
      display.print("PARI:");
      display.print(pari);
      display.setCursor(0, 33);
      display.print("DATA:");
      display.print(data);
      display.setCursor(80, 33);
      display.print("STOP:");
      display.print(stop);
      display.setCursor(0, 44);
      display.print("FLOW:");
      display.print(flow);
    break;
  }
}

void BDRDisplay::tela_saida(char* tensao, int Ssaida, char* pacot, char* Slaten) {
  display.fillRect(0, 10, 128, 45, 0); 
  display.setCursor(25, 11);
  display.print("SAIDA STATUS");
  display.setCursor(0, 22);
  display.print("TENSAO:");
  display.print(tensao);
  display.setCursor(0, 33);
  display.print("STATUS:");
    if (Ssaida == 0) {
      display.print("DESCONECTADO");
    } else if (Ssaida == 1) {
      display.print("CONECTADO");
    }
  display.setCursor(0, 44);
  display.print("PACOT:");
  display.print(pacot);
  display.setCursor(65, 44);
  display.print("LATEN:");
  display.print(Slaten);
}

void BDRDisplay::tela_entrar_config() {
  display.fillRect(0, 10, 128, 45, 0); 
  display.setCursor(35, 11);
  display.print("IR PARA O");
  display.setCursor(40, 22);
  display.print("MODO DE");
  display.setCursor(25, 33);
  display.print("CONFIGURACAO?");
}

void BDRDisplay::tela_localizacao(char* lat, char* lon, char* alt, char* pre, char* utc) {
  display.fillRect(0, 10, 128, 45, 0); 
  display.setCursor(30, 11);
  display.print("LOCALIZACAO");
  display.setCursor(0, 22);
  display.print("LAT:");
  display.print(String(lat).substring(0, 5));
  display.setCursor(65, 22);
  display.print("LON:");
  display.print(String(lon).substring(0, 5));
  display.setCursor(0, 33);
  display.print("ALT:");
  display.print(String(alt).substring(0, 5));
  display.setCursor(65, 33);
  display.print("PRE");
  display.print(String(pre).substring(0, 5));
  display.setCursor(0, 44);
  display.print("UTC:");
  display.print(utc);
}

void BDRDisplay::display_fixo(int modo, int page, int statusOUT){
  display.fillRect(0, 0, 128, 10, 0);
  display.drawLine(0, 8, 128, 8, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MODO:");
  switch(modo){
    case 0:
      display.print("CONFIG");
    break;
    case 1:
      display.print("CLIENT");
    break;
    case 2:
      display.print("LOCAL");
    break;
    case 3:
      display.print("SOURCE");
    break;
  }
  display.setCursor(74, 0);
  display.print("SAIDA:");
  switch(statusOUT){
    case 0: display.print("OFF");
    break;
    case 1: display.print("5V");
    break;
    case 2: display.print("9V");
    break;
    case 3: display.print("12V");
    break;
    case 4: display.print("15V");
    break;
    case 5: display.print("20V");
    break;
    case 6: display.print("N/A");
    break;
  }
  switch(modo){
    case 0:
      if(page==2){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 1:
      if(page==5){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 2:
      if(page==5){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 3:
      if(page==4){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
  }
}

void BDRDisplay::tela_info_alimentacao(int PDvolts){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(13, 11);
  display.print("INFO ALIMENTACAO");
  display.setCursor(0, 22);
  display.print("STATUS:");
  if(PDvolts>1){
    display.print("USB PD");
  }else{
    display.print("APENAS 5V");
  }
  display.setCursor(0, 33);
  display.print("TENSOES DISPONIVEIS:");
  display.setCursor(0, 44);
  display.print("5V");
  if(PDvolts &(1 << 1)){
    display.print(", 9V");
  }
  if(PDvolts &(1 << 2)){
    display.print(", 12V");
  }
  if(PDvolts &(1 << 3)){
    display.print(", 15V");
  }
  if(PDvolts &(1 << 4)){
    display.print(", 20V");
  }
}

void BDRDisplay::tela_info_bdr(char* modelo, char* firmware, char* serial){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(40, 11);
  display.print("INFO BDR");
  display.setCursor(18, 22);
  display.print("HUGEN PLUS LTDA");
  display.setCursor(0, 33);
  display.print("MOD:");
  display.print(modelo);
  display.setCursor(60, 33);
  display.print("FIRM:");
  display.print(firmware);
  display.setCursor(0, 44);
  display.print("SN:");
  display.print(serial);
}

void BDRDisplay::display_config(int page, bool conectado, int statusPD, char* modelo, char* firmware, char* serial) {
  static bool state;
  static unsigned long lastBlinkTime1;
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextSize(1);
  switch(page){
    case 0:
      if (millis() - lastBlinkTime1 >= 2000){
        state =! state;
        lastBlinkTime1 = millis();
      }
      if (state) {
        display.fillRect(3, 12, 121, 9, 0);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(4, 11);
        display.println("MODO DE CONFIGURACAO");
      }else{
        display.fillRect(3, 12, 121, 9, 1);
        display.setTextColor(SSD1306_BLACK);
        display.setCursor(4, 11);
        display.println("MODO DE CONFIGURACAO");
      }
      display.setTextColor(SSD1306_WHITE);
      if(conectado == false){
        display.setCursor(13, 22);
        display.println("Conecte-se a rede");
        display.setCursor(25, 44);
        display.println("BDR-CFG-30:15");
      } else {
        display.setCursor(4, 22);
        display.println("Aplicativo Conectado");
        display.setCursor(16, 44);
        display.println("verifique o app!");
      }
    break;
    case 1:
      display.setCursor(19, 22);
      display.print("Sair do modo de");
      display.setCursor(25, 33);
      display.print("configuracao?");
    break;
    case 2:
      tela_info_alimentacao(statusPD);
    break;
    case 3:
      tela_info_bdr(modelo, firmware, serial);
    break;
  }
} 