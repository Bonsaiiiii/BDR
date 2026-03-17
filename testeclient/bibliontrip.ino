#include "WiFi.h"
#include "base64.hpp"

char cliente_data_ap[1024];
char ch_ap[1024];

WiFiServer ntripServer(2101);
WiFiClient client = ntripServer.available();
unsigned long currentMillis_ap;
unsigned long atualMillis_ap;
bool flag_ok_ap = false;
bool flag_send_ap;
unsigned long int readcount_ap;

String scrtbl(const char* mountpoint, String ip, uint16_t port){
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

int check_aut(String data, String user, String pwr){
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
  if(cliente_usuario == user && cliente_senha == pwr){
    return 8;// senha correta
  }else{
    return 9;// senha incorreta
  }
}

int check_ver(String data){
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

int check_mountpoint(String data, String mountpoint){
  int posicao_inicial = data.indexOf("/");
  int posicao_final = data.indexOf("HTTP");
  posicao_final--;
  String cliente_mountpoint = data.substring(posicao_inicial,posicao_final);
  String mp = "/";
  mp = mp + mountpoint;
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

void setup() {
  Serial.begin(115200);
  delay(100);
  IPAddress Ip(192, 168, 0, 1);
  IPAddress Iplocal(192, 168, 0, 1); 
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Iplocal, Ip, NMask);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ntripteste", NULL, 7, 0, 10);   //launch the access point
  ntripServer.begin();
}

void loop() {
  WiFiClient client = ntripServer.available();
  if (client) {

    while (client.connected()) {
      int counter = 0;
      
      while (client.available()>0) {
      
        cliente_data_ap[counter] = client.read();
        counter++;
        if(client.available()==0){        
          
          String c_data_s = String(cliente_data_ap);
          Serial.println(c_data_s);

          if(c_data_s.indexOf("$GPGGA")==-1){
            
            currentMillis_ap = millis();
            switch(check_ver(c_data_s)*check_mountpoint(c_data_s,"ntripteste")*check_aut(c_data_s,"user","passw")){
              case 40:  //Serial.println("REV1 + mount point correto + autentificação correta");// REV1 + mount point correto + autentificação correta
                        client.println("ICY 200 OK");
                        flag_ok_ap = true;
                        Serial.println("40");
                break;
              case 48:  //Serial.println("REV1 + solicita mount point + autentificação correta");// REV1 + solicita mount point + autentificação correta
                        client.println("SOURCETABLE 200 OK");
                        client.println(scrtbl("ntripteste",WiFi.softAPIP().toString(),2101));
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
                        client.println(scrtbl("mtptteste",WiFi.softAPIP().toString(),2101));
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
            long int size = sizeof(cliente_data_ap);
            memset(cliente_data_ap,' ',size*sizeof(char));
          }
          else{
            currentMillis_ap = millis();
            flag_send_ap=true;
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
/*#include <WiFi.h>
#include <WiFiClient.h>
#include <base64.h>

// WiFi Credentials
const char* ssid = "YourWiFiSSID";
const char* wifiPassword = "YourWiFiPassword";

// NTRIP Server Settings
const char* mntpnt = "RTK_CORRECTION";
const char* ntripUser = "admin";
const char* ntripPass = "password";
const int ntripPort = 2101;

// GPS Configuration
#define GPS_SERIAL Serial2  // UART2 (GPIO 16=RX, 17=TX on ESP32)
#define GPS_BAUD 115200

WiFiServer server(ntripPort);
bool isWiFiConnected = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial Monitor to open (for boards with native USB)
  
  Serial.println("\nBooting...");
  
  // Initialize GPS serial
  GPS_SERIAL.begin(GPS_BAUD);
  delay(1000);  // Let GPS initialize

  // Connect to WiFi
  connectToWiFi();
}

void loop() {
  // Reconnect WiFi if connection drops
  if (WiFi.status() != WL_CONNECTED && isWiFiConnected) {
    Serial.println("WiFi disconnected! Attempting to reconnect...");
    isWiFiConnected = false;
    connectToWiFi();
  }

  // Handle client connections
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    handleClient(client);
    client.stop();
    Serial.println("Client disconnected");
  }
}

// Connect/reconnect to WiFi
void connectToWiFi() {
  WiFi.disconnect(true);  // Reset WiFi state
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());
    server.begin();  // Restart server on new connection
    isWiFiConnected = true;
  } else {
    Serial.println("\nFailed to connect to WiFi!");
  }
}

// Handle client authentication and data streaming
void handleClient(WiFiClient &client) {
  String request = client.readStringUntil('\r');
  if (request.indexOf("GET /" + String(mntpnt)) != -1) {
    // Read headers
    while (client.connected()) {
      String header = client.readStringUntil('\r');
      if (header.startsWith("Authorization: Basic ")) {
        String receivedCreds = header.substring(21);  // Extract base64 credentials
        String expectedCreds = base64::encode(String(ntripUser) + ":" + String(ntripPass));
        
        if (receivedCreds.equals(expectedCreds)) {
          client.println("ICY 200 OK\r\n");
          streamRTCM(client);
        } else {
          client.println("HTTP/1.1 401 Unauthorized\r\n");
        }
        break;
      }
    }
  }
}

// Stream RTCM data from GPS
void streamRTCM(WiFiClient &client) {
  Serial.println("Streaming RTCM data...");
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 30000) {  // Timeout after 30s
    if (GPS_SERIAL.available()) {
      client.write(GPS_SERIAL.read());
      timeout = millis();  // Reset timeout on data transfer
    }
    delay(1);  // Prevent watchdog crashes
  }
  Serial.println("Streaming stopped");
} /*



/*#include <WiFi.h>
#include <Base64.h> // Install this library from the Arduino Library Manager

// Replace with your WiFi credentials
const char* ssid = "HugenPLUS";
const char* password = "H32851112";

// Replace with your NTRIP server details
const char* ntripServer = "rtk2go.com";
const int ntripPort = 2101;
const char* mountpoint = "/TEST";
const char* ntripUser = "";
const char* ntripPassword = "";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  connectToNTRIP();
}

void loop() {
  // If disconnected, reconnect to the NTRIP server
  if (!client.connected()) {
    Serial.println("Disconnected from NTRIP server. Reconnecting...");
    connectToNTRIP();
    delay(1000);
  }

  // Read and process data from the NTRIP server
  if (client.available()) {
    char c = client.read();
    // Handle the RTCM data here (e.g., forward it to a GPS module)
    Serial.print(c); // Print raw data (for debugging)
  }
}

void connectToNTRIP() {
  Serial.println("Connecting to NTRIP server...");

  if (client.connect(ntripServer, ntripPort)) {
    Serial.println("Connected to NTRIP server!");

    // Send HTTP GET request with NTRIP headers
    client.print("GET ");
    client.print(mountpoint);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(ntripServer);
    client.print("\r\n");
    client.print("Ntrip-Version: Ntrip/2.0\r\n");
    client.print("User-Agent: NTRIP-ArduinoClient/1.0\r\n");

    // Add authentication header if required
    if (strlen(ntripUser) > 0) {
      String auth = String(ntripUser) + ":" + String(ntripPassword);
      String authBase64 = base64::encode(auth); // Encode credentials
      client.print("Authorization: Basic ");
      client.print(authBase64);
      client.print("\r\n");
    }

    client.print("Connection: close\r\n");
    client.print("\r\n"); // End of headers

    Serial.println("Request sent to NTRIP server.");
  } else {
    Serial.println("Connection to NTRIP server failed!");
  }
} */



/*#include <WiFi.h>
WiFiServer wifiServer(2000);

void setup() {
  Serial.begin(115200);
  WiFi.begin("HugenPLUS", "H32851112");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  wifiServer.begin();
  Serial.println(WiFi.localIP());
}

const char* test = "Hello";

void loop() {
  WiFiClient client = wifiServer.available();
  
  if (client) { // Check if a new client is connected
    Serial.println("New client connected.");
    
    while (client.connected()) { // While the client is connected
      client.println(test); // Send the correct response
      Serial.println("test OK");
      
      delay(1000); // Wait 1 second
      
      // Check if data is available from the client (optional)
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.print("Received: ");
        Serial.println(request);
      }
    }
    
    client.stop(); // Close the connection properly
    Serial.println("Client disconnected.");
  }
} */

/*#include <WiFi.h>

WiFiServer wifiServer(80); // Use port 80 for HTTP

void setup() {
  Serial.begin(115200);
  WiFi.begin("HugenPLUS", "H32851112"); // Replace with your SSID and password

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  wifiServer.begin();
  Serial.println("WiFi connected!");
  Serial.print("Server IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = wifiServer.available(); // Check for incoming clients

  if (client) { // If a new client connects
    Serial.println("New client connected.");

    while (client.connected()) { // While the client is connected
      if (client.available()) { // Check if data is available from the client
        String request = client.readStringUntil('\r'); // Read the HTTP request
        Serial.print("Received: ");
        Serial.println(request);

        // Send an HTTP response
        client.println("HTTP/1.1 200 OK"); // HTTP status line
        client.println("Content-Type: text/html"); // Response header
        client.println("Connection: close"); // Close the connection after response
        client.println(); // End of headers
        client.println("<html><body><h1>Hello from Arduino!</h1></body></html>"); // Response body

        Serial.println("Response sent.");
        break; // Exit the loop after sending the response
      }
    }

    client.stop(); // Close the connection
    Serial.println("Client disconnected.");
  }
} */