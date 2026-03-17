#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

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
#define OLED_RESET     DRST // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS DSP_ADDR ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // -1 is for the reset pin if not used

const uint8_t bitmap_wifi[] = {0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x06, 0x00, 0x73, 0x00, 0x79, 0x00, 0x0d, 0x80, 0x6d, 0x80, 0x6d, 0x80, 0x00, 0x00};
const uint8_t bitmap_seta[] = {0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x64, 0x00, 0x63, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00};
const uint8_t bitmap_client[] = {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1f, 0x00, 0x0e, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x00, 0x00};
const uint8_t bitmap_logo[] = {0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0xc7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x83, 0xf8, 0xe0, 0xe3, 0x83, 0x83, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x03, 0xf8, 0xe0, 0xe3, 0x83, 0x87, 0xfc, 0x7f, 0x8e, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x3f, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xfc, 0x7f, 0x8f, 0x0e, 0x00, 0x30, 0x00, 0x00, 0xfc, 0x7e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x8f, 0xf8, 0x7f, 0x0f, 0x0e, 0xbc, 0x31, 0x82, 0x3c, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0x8e, 0xfe, 0x31, 0x82, 0x3e, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9e, 0x00, 0x70, 0x0f, 0xce, 0xc7, 0x31, 0x82, 0x62, 0xfc, 0x3e, 0x01, 0xf8, 0xe0, 0xe3, 0x83, 0x9c, 0x00, 0x70, 0x0f, 0xce, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf8, 0xff, 0xe3, 0x83, 0x9c, 0x00, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x60, 0xfc, 0x3e, 0x03, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8f, 0xee, 0x83, 0x31, 0x82, 0x30, 0xfc, 0x3e, 0x0f, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x3c, 0xff, 0xbe, 0xff, 0xf0, 0xff, 0xe3, 0x83, 0x9c, 0x7e, 0x7f, 0x8e, 0xfe, 0x83, 0x31, 0x82, 0x0e, 0xff, 0xbe, 0xff, 0xf0, 0xe0, 0xe3, 0x83, 0x9e, 0x7e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x82, 0x06, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0x83, 0x9e, 0x0e, 0x70, 0x0e, 0x7e, 0x83, 0x31, 0x86, 0x02, 0xff, 0xbe, 0xff, 0xe0, 0xe0, 0xe3, 0xc7, 0x1e, 0x0e, 0x70, 0x0e, 0x3e, 0x87, 0x31, 0xc6, 0x62, 0xff, 0xbe, 0xff, 0xc0, 0xe0, 0xe3, 0xff, 0x0f, 0xfe, 0x7f, 0x8e, 0x1e, 0xfe, 0x3c, 0xfe, 0x7e, 0xff, 0xbe, 0xff, 0x80, 0xe0, 0xe1, 0xff, 0x0f, 0xfe, 0x7f, 0xce, 0x1e, 0xfc, 0x3c, 0x7a, 0x3c, 0xff, 0xbe, 0xff, 0x00, 0xe0, 0xe1, 0xff, 0x07, 0xfc, 0x7f, 0x8e, 0x1e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xfe, 0x00, 0xe0, 0xe0, 0xfe, 0x03, 0xfc, 0x7f, 0x8e, 0x0e, 0x80, 0x00, 0x00, 0x00, 0xff, 0xbe, 0xf8, 0x00, 0xe0, 0xe0, 0x7c, 0x00, 0xf0, 0x03, 0x8c, 0x06, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x07, 0x03, 0x20, 0x3c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x08, 0xfc, 0x3e, 0x00, 0x00, 0xf8, 0x3c, 0x0f, 0x03, 0x20, 0x7c, 0x04, 0x03, 0xc0, 0x78, 0x10, 0x0c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0c, 0x03, 0x20, 0x64, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1c, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x03, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xc0, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x18, 0x02, 0xa0, 0x46, 0x04, 0x06, 0x60, 0xb8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x08, 0x02, 0xe0, 0x46, 0x04, 0x06, 0x60, 0xc8, 0x10, 0x1e, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x30, 0x0d, 0x02, 0x60, 0x6c, 0x04, 0x03, 0xe0, 0xc8, 0x10, 0x33, 0xfc, 0x3e, 0x00, 0x00, 0x20, 0x3c, 0x0f, 0x02, 0x60, 0x7c, 0x07, 0x03, 0xc0, 0x78, 0x10, 0x33, 0x7c, 0x3e, 0x00, 0x00, 0x20, 0x1c, 0x07, 0x02, 0x20, 0x38, 0x07, 0x01, 0x80, 0x70, 0x00, 0x23};

void pins_init(){
  pinMode(CF3, OUTPUT);
  pinMode(CF2, OUTPUT);
  pinMode(CF1, OUTPUT);
  pinMode(PDPG, INPUT);
  pinMode(LED2B, OUTPUT);
  pinMode(LED2R, OUTPUT);
  pinMode(LED2G, OUTPUT);
  pinMode(LED1B, OUTPUT);
  pinMode(LED1R, OUTPUT);
  pinMode(LED1G, OUTPUT);
  pinMode(PAGE, OUTPUT);
  pinMode(ENTER, OUTPUT);
  pinMode(POUT, OUTPUT);
  pinMode(DRST, OUTPUT);
  pinMode(DPWR, OUTPUT);
  pinMode(DET, INPUT);
}


void texto_fixo_inferior(int state) {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.fillRect(0, 56, 128, 64, 0);
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

int page2(int cursor) {
  int ycursor=0;
    display.fillRect(0, 10, 128, 45, 0);  
    display.setTextColor(WHITE);
    display.setTextSize(1);
    ycursor = cursor + 12;
    display.setCursor(0, ycursor);
    display.print("NTRIPHost:");
    display.print("169.168.4.1");
    ycursor += 10;
    display.setCursor(0, ycursor);
    display.print("Port:");
    display.print("2101");
    ycursor += 10;
    display.setCursor(0, ycursor);
    display.print("User:");
    display.print("usuario");
    ycursor += 10;
    display.setCursor(0, ycursor);
    display.print("Password:");
    display.print("senha");
    ycursor += 10;
    display.setCursor(0, ycursor);
    display.print("MountPoint:");
    display.print("BDR0");
    //display.display();
    return ycursor;
}

void display_fix_part(int modo, int pagina, int statusOUT){
  display.fillRect(0, 0, 128, 10, 0);
  display.drawLine(0, 8, 128, 8, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MODO:");
  switch(modo){
    case 0: //modo de configuração
      display.print("Config");
    break;
    case 1: //modo cliente
      display.print("Client");
    break;
    case 2: //modo local
      display.print("Local");
    break;
    case 3: //modo source
      display.print("Source");
    break;
  }
  display.setCursor(74, 0);
  display.print("Saida:");
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
      if(pagina==1){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 1:
      if(pagina==5){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 2:
      if(pagina==4){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
    case 3:
      if(pagina==4){
        texto_fixo_inferior(1);
      }else{
        texto_fixo_inferior(0);
      }
    break;
  }
}
void tela_info_alimentacao(int PDvolts){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(13, 12);
  display.print("INFO Alimentacao");
  display.setCursor(0, 23);
  display.print("Status: ");
  if(PDvolts>1){
    display.print("USB PD");
  }else{
    display.print("only 5V");
  }
  display.setCursor(4, 35);
  display.print("Tensoes disponiveis:");
  display.setCursor(0, 45);
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

void tela_info_bdr(char* modelo, char* firmware, char* serial){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(40, 12);
  display.print("INFO BDR");
  display.setCursor(0, 22);
  display.print("Hugen PLUS LTDA");
  display.setCursor(0, 32);
  display.print("Mod:BDR>01  FIRM:V1.5");
  display.print(modelo);
  display.print("  FIRM:");
  display.print(firmware);
  display.setCursor(0, 42);
  display.print("SN: ");
  display.print(serial);
}

void testa_pd(int PDvolts){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(13, 26);
  display.print(PDvolts, BIN);
  display.display();
}

void display_config(int page, bool conectado, int statusPD, char* modelo, char* firmware, char* serial) {
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
        display.setCursor(4, 13);
        display.println("MODO DE CONFIGURACAO");
      }else{
        display.fillRect(3, 12, 121, 9, 1);
        display.setTextColor(SSD1306_BLACK);
        display.setCursor(4, 13);
        display.println("MODO DE CONFIGURACAO");
      }
      display.setTextColor(SSD1306_WHITE);
      if(conectado == false){
        display.setCursor(13, 26);
        display.println("Conecte-se a rede");
        display.setCursor(25, 41);
        display.println("BDR-CFG-30:15");
      } else {
        display.setCursor(4, 26);
        display.println("Aplicativo Conectado");
        display.setCursor(16, 41);
        display.println("verifique o app!");
      }
    break;
    case 1:
      display.setCursor(19, 20);
      display.print("Sair do modo de");
      display.setCursor(25, 36);
      display.print("configuracao?");
    break;
    case 2:
      tela_info_alimentacao(statusPD);
    break;
    case 3:
      tela_info_bdr(modelo, firmware, serial);
    break;
  }
  display.display();
}

void usbpd(int volt){
  switch(volt){
    case 5:
      digitalWrite(CF1,HIGH);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
    break;
    case 9:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,LOW);
    break;
    case 12:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,LOW);
      digitalWrite(CF3,HIGH);
    break;
    case 15:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,HIGH);
    break;
    case 20:
      digitalWrite(CF1,LOW);
      digitalWrite(CF2,HIGH);
      digitalWrite(CF3,LOW);
    break;
  }
}

int test_pd(){
  static int state = 1;
  usbpd(9);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 1;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 1;
  }
  usbpd(12);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 2;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 2;
  }
  usbpd(15);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 3;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 3;
  }
  usbpd(20);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 4;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 4;
  }
  usbpd(5);
  return state;
}

/*int test_pd(){
  static int state = 1;
  usbpd(9);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 1;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 1;
  }
  usbpd(12);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 2;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 2;
  }
  usbpd(15);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 3;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 3;
  }
  usbpd(20);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    state |= 1 << 4;
  }else if(digitalRead(PDPG)==HIGH){
    state |= 0 << 4;
  }
  usbpd(5);
  return state;
} */

/*int test_pd(){
  static int oldMillis = 0;
  static int state = 1;
  static int readNum = 0;
  int newMillis = millis();
  if (newMillis - oldMillis == 300) {
    readNum + 1;
    oldMillis = newMillis;
    if (readNum > 4) {
      readNum = 0;
    }
    Serial.println(state);
  }
  if (readNum = 0) {
    usbpd(9);
  }
  if (readNum = 1) {
    if(digitalRead(PDPG)==LOW){
      state |= 1 << 1;
    } else if(digitalRead(PDPG)==HIGH){
      state |= 0 << 1;
    }
    usbpd(12);
  }
  if (readNum = 2) {
    if(digitalRead(PDPG)==LOW){
      state |= 1 << 2;
    }else if(digitalRead(PDPG)==HIGH){
      state |= 0 << 2;
    }
    usbpd(15);
  }
  if (readNum = 3) {
    if(digitalRead(PDPG)==LOW){
      state |= 1 << 3;
    }else if(digitalRead(PDPG)==HIGH){
      state |= 0 << 3;
    }
    usbpd(20);
  }
  if (readNum = 4) {
    if(digitalRead(PDPG)==LOW){
      state |= 1 << 4;
    }else if(digitalRead(PDPG)==HIGH){
      state |= 0 << 4;
    }
    usbpd(5);
  }
  return state;
} */

void setup() {
  pins_init();
  digitalWrite(LED1B,HIGH);
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

int a, b, c; 
int cursor, ymax = 45, dir;
void loop() {
  /*
  Serial.print("cursor: ");Serial.println(cursor);
  a = page2(cursor);
  texto_fixo_inferior(0);
  texto_fixo_superior(0, 0);
  display.display();
  if(!dir){
    cursor--;
  }
  if(dir){
    cursor++;
  }
  if (a == ymax){
    dir=1;
  }
  if(cursor==0){
    dir=0;
  }
  delay(1000);
  */
  //tela_config_inicial(0);
  int pd = test_pd();
  tela_info_alimentacao(pd);
  display.display();
  Serial.println(pd);
}



void modo_config_display(){
  static int pagina_conf;
 // if(button_page){
    pagina_conf++;
    //button_page = 0;
  //}
  switch(pagina_conf){
    case 0:  
      //tela_config(statusAPP);   
    break;
    case 1:
      display.fillRect(0, 10, 128, 45, 0);  
      display.setTextColor(WHITE);
      display.setTextSize(1);
      
    break;
  }
  
}
