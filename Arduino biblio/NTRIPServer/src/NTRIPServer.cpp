#include "NTRIPServer.h"
#include "base64.hpp"

NTRIPServer::NTRIPServer(uint16_t port) : _ntripServer(port) {
    // Constructor initializes the server with the specified port
}

void NTRIPServer::begin(const char* mountpoint, const char* user, const char* password, 
                        const char* ssid, const char* apPassword, 
                        IPAddress ip, IPAddress gateway, IPAddress subnet) {
    _mountpoint = mountpoint;
    _user = user;
    _password = password;
    _ssid = ssid;
    _apPassword = apPassword;
    _ip = ip;
    _gateway = gateway;
    _subnet = subnet;

    Serial.begin(115200);
    delay(100);
    WiFi.softAPConfig(_gateway, _ip, _subnet);
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_ssid, _apPassword, 7, 0, 10);
    _ntripServer.begin();
}

void NTRIPServer::handle() {
    WiFiClient client = _ntripServer.available();
    if (client) {
        while (client.connected()) {
            int counter = 0;
            while (client.available() > 0) {
                cliente_data_ap[counter] = client.read();
                counter++;
                if (client.available() == 0) {
                    String c_data_s = String(cliente_data_ap);
                    Serial.println(c_data_s);

                    if (c_data_s.indexOf("$GPGGA") == -1) {
                        currentMillis_ap = millis();
                        int authResult = checkAuthentication(c_data_s, "user", "passw");
                        int verResult = checkHTTPVersion(c_data_s);
                        int mountResult = checkMountpoint(c_data_s);

                        switch (verResult * mountResult * authResult) {
                            case 40:  //Serial.println("REV1 + mount point correto + autentificação correta");// REV1 + mount point correto + autentificação correta
                                    client.println("ICY 200 OK");
                                    flag_ok_ap = true;
                                    Serial.println("40");
                            break;
                        case 48:  //Serial.println("REV1 + solicita mount point + autentificação correta");// REV1 + solicita mount point + autentificação correta
                                    client.println("SOURCETABLE 200 OK");
                                    client.println(generateSourceTable("ntripteste",WiFi.softAPIP().toString(),2101));
                                    //Serial.println(scrtbl(cMNTPL,WiFi.softAPIP().toString(),port));
                                    client.stop();
                                    Serial.println("48");
                            break;
                        case 56:  //Serial.println("REV1 + mount point incorreto + autentificação correta");// REV1 + mount point incorreto + autentificação correta
                                    flag_ok_ap = false;
                            break;
                        case 45:  //Serial.println("REV1 + mount point correto + autentificação incorreta");// REV1 + mount point correto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 54:  //Serial.println("REV1 + solicita mount point + autentificação incorreta");// REV1 + solicita mount point + autentificação incorreta
                                    flag_ok_ap = false;
                            break; 
                        case 63:  //Serial.println("REV1 + mount point incorreto + autentificação incorreta");// REV1 + mount point incorreto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 80:  //Serial.println("REV2 + mount point correto + autentificação correta");// REV2 + mount point correto + autentificação correta
                                    client.write("HTTP/1.1 200 OK\r\n");
                                    flag_ok_ap = true;
                                    //Serial1.begin(SBAUDL.toInt(), SERIAL_8N1, RXDT, TXDT);
                                    //digitalWrite(TXCF,HIGH);
                            break;
                        case 96:  //Serial.println("REV2 + solicita mount point + autentificação correta");// REV2 + solicita mount point + autentificação correta
                                    client.println("SOURCETABLE 200 OK");
                                    client.println(generateSourceTable("mtptteste",WiFi.softAPIP().toString(),2101));
                                    //Serial.println(scrtbl(cMNTPL,WiFi.softAPIP().toString(),port));
                                    client.stop();
                            break;
                        case 112: //Serial.println("REV2 + mount point incorreto + autentificação correta");// REV2 + mount point incorreto + autentificação correta
                                    flag_ok_ap = false;
                            break;
                        case 90:  //Serial.println("REV2 + mount point correto + autentificação incorreta");// REV2 + mount point correto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 108: //Serial.println("REV2 + solicita mount point + autentificação incorreta");// REV2 + solicita mount point + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 126: //Serial.println("REV2 + mount point incorreto + autentificação incorreta");// REV2 + mount point incorreto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;

                        case 160: //Serial.println("NAO HTTP + mount point correto + autentificação correta");// NAO HTTP + mount point correto + autentificação correta
                                    flag_ok_ap = false;
                            break;
                        case 192: //Serial.println("NAO HTTP + solicita mount point + autentificação correta");// NAO HTTP + solicita mount point + autentificação correta
                                    flag_ok_ap = false;
                            break;
                        case 224: //Serial.println("NAO HTTP + mount point incorreto + autentificação correta");// NAO HTTP + mount point incorreto + autentificação correta
                                    flag_ok_ap = false;
                            break;
                        case 180: //Serial.println("NAO HTTP + mount point correto + autentificação incorreta");// NAO HTTP + mount point correto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 216: //Serial.println("NAO HTTP + solicita mount point + autentificação incorreta");// NAO HTTP + solicita mount point + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        case 252: //Serial.println("NAO HTTP + mount point incorreto + autentificação incorreta");// NAO HTTP + mount point incorreto + autentificação incorreta
                                    flag_ok_ap = false;
                            break;
                        }
                        memset(cliente_data_ap, 0, sizeof(cliente_data_ap));
                    } else {
                        currentMillis_ap = millis();
                        flag_send_ap = true;
                    }
                }
            }
            atualMillis_ap = millis();
      if(atualMillis_ap - currentMillis_ap > 2500 && flag_ok_ap == true && currentMillis_ap != 0 && flag_send_ap==true){
          client.stop();
          //Serial.println(atualMillis_ap - currentMillis_ap);
          currentMillis_ap = 0;
          atualMillis_ap = 0;
          //Serial.println("Client disconnected timeout");
          
          long int size = sizeof(cliente_data_ap);
          memset(cliente_data_ap,' ',size*sizeof(char));
          flag_send_ap=false;
          //ESP.restart();
      }      
      while (Serial.available() && flag_ok_ap == true) {
        readcount_ap = 0;
        while (Serial.available()) {
          ch_ap[readcount_ap] = Serial.read();
          readcount_ap++;
          if (readcount_ap > 511)break;
        }//buffering
        
        client.write((uint8_t*)ch_ap, readcount_ap);
        //Serial.println(readcount_ap);
      } 
    }  
    delay(300);
    client.stop();
    //Serial.println("Client disconnected");
    flag_ok_ap == false;
    long int size = sizeof(cliente_data_ap);
    currentMillis_ap = 0;
    atualMillis_ap = 0;
    memset(cliente_data_ap,' ',size*sizeof(char));
    flag_send_ap=false;
    }
}

String NTRIPServer::generateSourceTable(const char* mountpoint, String ip, uint16_t port) {
    String body = "STR;";
    body = body + mountpoint + String(";;;;0;;HUGEN;BRA;0.00;0.00;0;0;sNTRIP;none;N;N;0;none;\r\n"
            "NET;HUGEN;;N;N;None;") + ip + String(":") + port + String(";None;;\r\n"
            "ENDSOURCETABLE\r\n");
    uint16_t tamanho = body.length();
    String header = "Server:"; 
    header = header + String(" Hugenplus ntrip simple ntrip caster/R1.0\r\n"
            //"Date: ") + ntp.getFormattedDate() + String("\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: ") + String(tamanho) + String("\r\n");
    return header + String("\r\n") + body + String("\r\n");
}

int NTRIPServer::checkAuthentication(String data, String user, String pwr) {
    char base64[50];
    unsigned char string[50];
    //Serial.print("user: ");Serial.println(user);
    //Serial.print("pwr: ");Serial.println(pwr);
    int posicao = data.indexOf(": Basic");
    posicao = posicao + 8;
    String usuario_encoded = data.substring(posicao);
    usuario_encoded.toCharArray(base64,50);
    unsigned int string_length = decode_base64((unsigned char *)base64, string);
    string[string_length] = '\0';
    String usuario_decoded = String((char *)string);
    int separador = usuario_decoded.indexOf(":");
    String cliente_usuario = usuario_decoded.substring(0,separador);
    separador ++;
    String cliente_senha = usuario_decoded.substring(separador);
    //Serial.print("cliente_usuario: ");Serial.println(cliente_usuario);
    //Serial.print("cliente_senha: ");Serial.println(cliente_senha);
    if(cliente_usuario == _user && cliente_senha == pwr){
        return 8;// senha correta
    } else{
        return 9;// senha incorreta
    }
}

int NTRIPServer::checkHTTPVersion(String data) {
    int posicao_inicial = data.indexOf("HTTP");
    int posicao_final = data.indexOf("\r\n");
    if (posicao_inicial < 0){
        return 4; //ERRO1 
    }
    String versao_http = data.substring(posicao_inicial, posicao_final);
    //Serial.println(versao_http);
    if(versao_http == "HTTP/1.0"){
        return 1; //REV1
    }
    if(versao_http == "HTTP/1.1"){
        return 2; //REV2
    }
}

int NTRIPServer::checkMountpoint(String data) {
    int posicao_inicial = data.indexOf("/");
    int posicao_final = data.indexOf("HTTP");
    posicao_final--;
    String cliente_mountpoint = data.substring(posicao_inicial,posicao_final);
    String mp = "/";
    mp = mp + _mountpoint;
    //Serial.println(cliente_mountpoint);
    //Serial.println(mp);
    if(cliente_mountpoint == mp){
        return 5; //igual
    }
    if(cliente_mountpoint == "/SOURCETABLE.TXT " || cliente_mountpoint == "/"){
        return 6; //solicita
    }
        return 7; //ERRO2
}