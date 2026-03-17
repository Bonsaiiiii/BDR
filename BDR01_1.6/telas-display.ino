unsigned long lastBlinkTime = 0;  // Controle do tempo para piscadas
bool app_conected = false; 

/// função de atualização do display
void attDisplay(){
  devices=WiFi.softAPgetStationNum();
  if(devices!=devicesOld){
    attpage=true;
    devicesOld=devices;
  }
  if(attpage==true){
    delay(300);
    switch(a){
      case 0: page1();
              flag_page3=false;
              flag_page4=false;
      break;
      case 1: page2();
              flag_page3=false;
              flag_page4=false;
      break;
      case 2: page3();
              flag_page3=true;
              flag_page4=false;
      break;
      case 3: page4();
              flag_page3=false;
              flag_page4=true;
      break;
    }
    attpage=false;
  }
  if(flag_page3&&enter&& !flag_select_serial){
    for(int q = 0; q<5 ; q++){
      if(BAUDS[q]==SBAUD){
        b = q;
      }
    }
    flag_select_serial = true;
    attfixo=true;
    attpage3=true;
    pausa=true;
    enter=false;
  }
  if(flag_page4 && enter && !flag_select_power){
    OldPDvolt = PDvolt;
    for(int w = 0; w<5; w++){
      if(volt[w]==PDvolt){
        c = w;
      }
    }
    flag_select_power=true;
    attfixo=true;
    attpage4=true;
    pausa=true;
    enter=false;
  }
  if(attpage3==true){
    page3select(BAUDS[b]);
    if(enter && flag_select_serial){
      SBAUD = BAUDS[b];
      saveConfigFile(BDR_data);
      Serial1.begin(SBAUD.toInt(), SERIAL_8N1, RX, TX);
      attfixo=true;
      flag_select_serial=false;
      pausa=false;
      enter=false;
    }
    attpage3=false;
  }
  if(attpage4==true){
    page4select(volt[c]);
    if(enter && flag_select_power){
      if(!flag_confirm){
        page4confirm(volt[c]);
        fixoConfirm();
      }
      flag_confirm=true;
      if(enter && flag_select_power && confirm){
        PDvolt = volt[c];
        saveConfigFile(BDR_data);
        page4();
        attfixo = true;
        flag_select_power = false;
        pausa=false;
        enter=false;
        confirm=false;
        flag_confirm=false;
        if(digitalRead(DET)==LOW){
          if(V5&&PDvolt=="5V"){
            usbpd(5);
            stateOUT=1;
            attfixo=true;
            digitalWrite(LED1B,LOW);
            digitalWrite(LED1R,LOW);
            digitalWrite(LED1G,HIGH); 
          }
          if(V9&&PDvolt=="9V"){
            usbpd(9);
            stateOUT=1;
            attfixo=true;
            digitalWrite(LED1B,LOW);
            digitalWrite(LED1R,LOW);
            digitalWrite(LED1G,HIGH); 
          }
          if(V12&&PDvolt=="12V"){
            usbpd(12);
            stateOUT=1;
            attfixo=true;
            digitalWrite(LED1B,LOW);
            digitalWrite(LED1R,LOW);
            digitalWrite(LED1G,HIGH); 
          }
          if(V15&&PDvolt=="15V"){
            usbpd(15);
            stateOUT=1;
            attfixo=true;
            digitalWrite(LED1B,LOW);
            digitalWrite(LED1R,LOW);
            digitalWrite(LED1G,HIGH); 
          }
          if(V20&&PDvolt=="20V"){
            usbpd(20);
            stateOUT=1;
            attfixo=true;
            digitalWrite(LED1B,LOW);
            digitalWrite(LED1R,LOW);
            digitalWrite(LED1G,HIGH); 
          }
        }
      }
      if(enter && flag_select_power && cansel){
        PDvolt = OldPDvolt;
        saveConfigFile(BDR_data);
        page4();
        attfixo = true;
        flag_select_power = false;
        pausa=false;
        enter=false;
        cansel=false;
        flag_confirm=false;
      }
    }
    attpage4=false;
  }
  if(attfixo==true){
    delay(300);
    if(flag_select_serial){
      fixo2();
    }else if (flag_select_power){
      fixo2();
    }else{
      fixo();
    } 
    attfixo=false;
    if(stateOUT==1){
      digitalWrite(POUT,HIGH);
    }
  }
}


/// telas modo config
void tela_config_inicial() {
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextSize(1);
  if (millis() - lastBlinkTime >= 500) {
    display.fillRect(3, 12, 121, 9, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 13);
    display.println("MODO DE CONFIGURACAO");
    lastBlinkTime = millis();
  }else{
    display.fillRect(3, 12, 121, 9, 1);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(4, 13);
    display.println("MODO DE CONFIGURACAO");
  }
  display.setTextColor(SSD1306_WHITE);
  if(app_conected == false){
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
  display.display();
}

/// parte fixa na tela
void texto_fixo_superior() {
    display.fillRect(0, 0, 128, 8, 0);
    display.drawLine(0, 8, 128, 8, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("MODO:");
    display.print(MODO);
    display.setCursor(74, 0);
    display.print("Saida:");
    switch(stateOUT){
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
    display.display();
}

void texto_fixo_inferior(String aa) {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.fillRect(0, 56, 128, 64, 0);
    display.drawLine(0, 55, 128, 55, 1);
    if(aa=="confirma"){
      display.setCursor(0, 57);
      display.print("NAO");
      display.setCursor(98, 57);
      display.print("SIM");
    }else{
      display.setCursor(0, 57);
      display.print("PAGINA");
      display.setCursor(98, 57);
      display.print("ENTER");
    }
    display.display();
}

void pagemode(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(35, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print("WIFI INFO");
    display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
    display.setCursor(12, 21);
    display.print("NOME:");
    display.print(ssid);
    display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
    display.setCursor(12, 33);
    display.print("SENHA:");
    display.print(password);
    display.drawBitmap(0, 42, bitmap_client, 11, 11, 1);
    display.setCursor(12, 44);
    display.print("Clientes:");
    display.print(WiFi.softAPgetStationNum());
    display.display();
}
void page_status(){
  display.fillRect(0, 10, 128, 45, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(25, 11);
  display.println("Status BDR>01");
  
}

void page_wifi(String mode_wifi){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(35, 12);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    if(mode_wifi == "AP"){
      display.setCursor(10, 11);
      display.println("Conecte-se a rede:");
      display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
      display.setCursor(12, 21);
      display.print("NOME:");
      display.print(AP_SSID);
      display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
      display.setCursor(12, 33);
      display.print("SENHA:");
      display.print(AP_PASS);
      display.drawBitmap(0, 42, bitmap_client, 11, 11, 1);
      display.setCursor(12, 44);
      display.print("Clientes:");
      display.print(WiFi.softAPgetStationNum());
    }
    if(mode_wifi == "STA"){
      switch(WIFI_STATUS){
        case 0:
          display.fillRect(10, 10, 105, 9, 0);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(13, 11);
          display.println("Conectado a rede:");
        break;
        case 1:
          if (millis() - lastBlinkTime >= 500) {
            display.fillRect(8, 21, 110, 9, 0);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(10, 11);    
            display.println("Conectando a rede:");
            lastBlinkTime = millis();
          }else{
            display.fillRect(8, 21, 110, 9, 1);
            display.setTextColor(SSD1306_BLACK);
            display.setCursor(10, 11);    
            display.println("Conectando a rede:");
          }
        break;
        case 2;
          if (millis() - lastBlinkTime >= 500) {
            display.fillRect(2, 10, 122, 9, 0);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(4, 11);
            display.println("Rede Nao encontrada!");
            lastBlinkTime = millis();
          else{
            display.fillRect(2, 10, 122, 9, 1);
            display.setTextColor(SSD1306_BLACK);
            display.setCursor(4, 11);
            display.println("Rede Nao encontrada!");
          }
        break;
        case 3:
          if (millis() - lastBlinkTime >= 500) {
            display.fillRect(13, 10, 99, 9, 0);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(16, 11);
            display.println("Senha incorreta!");
            lastBlinkTime = millis();
          }else{
            display.fillRect(13, 10, 99, 9, 1);
            display.setTextColor(SSD1306_BLACK);
            display.setCursor(16, 11);
            display.println("Senha incorreta!");
          }
        break;
      }
      display.drawBitmap(0, 21, bitmap_wifi, 10, 10, 1);
      display.setCursor(12, 21);
      display.print("NOME:");
      display.print(STA_SSID);
      display.drawBitmap(0, 31, bitmap_seta, 10, 10, 1);
      display.setCursor(12, 33);
      display.print("SENHA:");
      display.print(STA_PASS);
      display.drawBitmap(0, 42, bitmap_client, 11, 11, 1);
      display.setCursor(12, 44);
      display.print("IP:");
      display.print(WiFi.softAPIP());
    }
    display.display();
}

void page_ntrip(String mod_ntrip) {
    if(mod_ntrip == "cliente"){

    }
    if(mod_ntrip == "source"){

    }
    if(mod_ntrip == "local"){
      display.fillRect(0, 10, 128, 45, 0);
      display.setCursor(0, 12);
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.print("NTRIPHost:");
      display.print(WiFi.softAPIP());
      display.setCursor(0, 22);
      display.print("Port:");
      display.print(port);
      display.setCursor(65, 22);
      display.print("User:");
      display.print(usuario);
      display.setCursor(0, 32);
      display.print("Password:");
      display.print(senha);
      display.setCursor(0, 42);
      display.print("MountPoint:");
      display.print(mountpoint);
      display.display();
    }
    
}

void page3() {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.setCursor(66, 22);
    display.print(SBAUD);
    display.setCursor(0, 32);
    display.print("Parity:None    Data:8");
    display.setCursor(0, 42);
    display.print("Stop: 1     Flow:None");
    display.display();
}
void page3select(String SB) {
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(12, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Porta Serial INFO");
    display.setCursor(0, 22);
    display.print("Baud Rate:");
    display.setCursor(66, 22);
    display.print(SB);
    display.setCursor(0, 32);
    display.print("Parity:None    Data:8");
    display.setCursor(0, 42);
    display.print("Stop: 1     Flow:None");
    display.display();
}

void page4(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Model:BDR01 FIRM:V1.5");
    display.setCursor(0, 22);
    display.print("Local NTRIP Bridge");
    display.setCursor(0, 32);
    display.print("power output:fix(aut)");
    display.setCursor(0, 42);
    display.print("Voltage set:");
    display.setCursor(74,42);
    display.print(PDvolt);
    display.display();
}

void page4select(String PDv){
  display.fillRect(0, 10, 128, 45, 0);
  display.setCursor(0, 12);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Model:BDR01 FIRM:V1.5");
  display.setCursor(0, 22);
  display.print("Local NTRIP Bridge");
  display.setCursor(0, 32);
  display.print("power output:fix(aut)");
  display.setCursor(0, 42);
  display.print("Voltage set:");
  display.setCursor(74,42);
  display.print(PDv);
  display.display();
}

void page4confirm(String PDvC){
  display.fillRect(0, 10, 128, 45, 0);
  display.setCursor(0, 12);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Warning check the max");
  display.setCursor(0, 22);
  display.print("voltage that the GNSS");
  display.setCursor(0, 32);
  display.print("receiver can handle!");
  display.setCursor(0, 42);
  display.print("Voltage set:");
  display.setCursor(74,42);
  display.print(PDvC);
  display.display();
}

void page5(){
    display.fillRect(0, 10, 128, 45, 0);
    display.setCursor(0, 12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Entrar em Modo");
    display.setCursor(0, 22);
    display.print("de configuração?");
    display.display();
}


void page_pd(){
    display.fillRect(0, 0, 128, 64, 0);
    display.setCursor(5, 2);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print("Fonte de Alimentacao");
    display.setCursor(10, 12);
    display.print("USB Power Delivery");
    display.setCursor(30, 22);
    display.print("Compativel?");
    display.setCursor(20, 32);
    display.print("9V:");
    if(V9==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(70, 32);
    display.print("12V:");
    if(V12==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(17, 42);
    display.print("15V:");
    if(V15==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.setCursor(67, 42);
    display.print("20V:");
    if(V20==HIGH){
      display.print("SIM");
    }else{
      display.print("NAO");
    }
    display.display();
}

void test_pd(){
  usbpd(9);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V9=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V9=LOW;
  }
  usbpd(12);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V12=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V12=LOW;
  }
  usbpd(15);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V15=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V15=LOW;
  }
  usbpd(20);
  delay(300);
  if(digitalRead(PDPG)==LOW){
    V20=HIGH;
  }else if(digitalRead(PDPG)==HIGH){
    V20=LOW;
  }
  if(V9==LOW&&V12==LOW&&V15==LOW&&V20==LOW){
    stateOUT=2;
  }
  usbpd(5);
  V5=HIGH;
}