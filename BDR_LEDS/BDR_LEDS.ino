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

void LedFunc(int LED1, int LED2) {
  digitalWrite(LED1R,LOW);
  digitalWrite(LED1G,LOW);
  digitalWrite(LED1B,LOW);
  digitalWrite(LED2R,LOW);
  digitalWrite(LED2G,LOW);
  digitalWrite(LED2B,LOW);
  switch(LED1) {
    case 0: //preto
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,LOW);
    break;
    case 1: //vermelho
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,LOW);
    break;
    case 2: //verde
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,LOW);
    break;
    case 3: //azul
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,HIGH);
    break;
    case 4: //amarelo
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,LOW);
    break;
    case 5: //rosa
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,LOW);
      digitalWrite(LED1B,HIGH);
    break;
    case 6: //ciano
      digitalWrite(LED1R,LOW);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,HIGH);
    break;
    case 7: //branco
      digitalWrite(LED1R,HIGH);
      digitalWrite(LED1G,HIGH);
      digitalWrite(LED1B,HIGH);
    break;
  }
  switch(LED2) {
    case 0: //preto
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,LOW);
    break;
    case 1: //vermelho
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,LOW);
    break;
    case 2: //verde
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,LOW);
    break;
    case 3: //azul
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,HIGH);
    break;
    case 4: //amarelo
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,LOW);
    break;
    case 5: //rosa
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,LOW);
      digitalWrite(LED2B,HIGH);
    break;
    case 6: //ciano
      digitalWrite(LED2R,LOW);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,HIGH);
    break;
    case 7: //branco
      digitalWrite(LED2R,HIGH);
      digitalWrite(LED2G,HIGH);
      digitalWrite(LED2B,HIGH);
    break;
  }
}

void LedBlink(int LED1, int LED2) {
  static int lastmillisled = 0;
  int currentmillisled = millis();
  if (currentmillisled - lastmillisled >= 500) {
    LedFunc(LED1, LED2);
    if (currentmillisled - lastmillisled >= 1000) {
      LedFunc(LED1, 0);
      lastmillisled = currentmillisled;
    }
  }
}

void LedConf() {
  static int RColor = 0;
  static int GColor = 0;
  static int BColor = 0;
  static int ColorIndex;

  static int lastmillisled = 0;
  int currentmillisled = millis();
  if (currentmillisled - lastmillisled > 500) {
    switch(ColorIndex) {
      case 0: // vermelho
        LedFunc(7, 1);
        ColorIndex = 1;
      break;
      case 1: //amarelo
        LedFunc(7, 4);
        ColorIndex = 2;
      break;
      case 2: //verde
        LedFunc(7, 2);
        ColorIndex = 3;
      break;
      case 3: //ciano
        LedFunc(7, 6);
        ColorIndex = 4;
      break;
      case 4: //azul
        LedFunc(7, 3);
        ColorIndex = 5;
      break;
      case 5: //rosa
        LedFunc(7, 5);
        ColorIndex = 6;
      break;
      case 6: //branco
        LedFunc(7, 7);
        ColorIndex = 0;
      break;
    }
    lastmillisled = currentmillisled;
  }
}

void setup() {
  pins_init();
  LedFunc(7, 0);
  Serial.begin(115200);
}

bool ledconf = false;
void loop() {
  //LedBlink(2, 3);
  if (digitalRead(ENTER) == HIGH) {
    ledconf = true;
  }
  if (digitalRead(PAGE) == HIGH) {
    ledconf = false;
  }
  if (ledconf) {
    LedConf();
  }
}
