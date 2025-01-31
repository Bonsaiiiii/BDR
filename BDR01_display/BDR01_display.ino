#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Scroller.h"
#include "BDR01_display.h"

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

BDRDisplay displaybdr;

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

void setup () {
  pins_init();
  digitalWrite(LED1B,HIGH);
  Wire.begin(SDA,SCL);
  Serial.begin(115200);
  displaybdr.initDisplay();
  if (!displaybdr.display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  displaybdr.display.clearDisplay();
  displaybdr.scrollerInitClientSource("12341234123412341234", "didi heroi da nacao", "aoba123456789");
}

int currentPage = 0;
void loop () {
  if (digitalRead(PAGE) == HIGH) {
    currentPage = currentPage + 1;
    delay(200);
  }
  if (currentPage > 8) {
       currentPage = 0;
  }
  //displaybdr.display_config(0, 0, NULL, NULL, NULL, NULL);
  displaybdr.page_client(currentPage, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  //displaybdr.page_source(currentPage, 3, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  displaybdr.display.display();
}