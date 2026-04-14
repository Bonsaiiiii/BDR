#include "ESP2SOTA.h"
#include "index_html.h"
//Class constructor
ESP2SOTAClass::ESP2SOTAClass(){

}

#if defined(ESP8266)
  void ESP2SOTAClass::begin(ESP8266WebServer *server){
#elif defined(ESP32)
  void ESP2SOTAClass::begin(AsyncWebServer *server){
#endif
_server = server;

// Serve the index.html OTA UI
_server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/html", indexHtml);
});

// Handle the firmware upload
_server->on("/update", HTTP_POST, 
  [](AsyncWebServerRequest *request) {}, 
  [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if (!index) {
      Serial.printf("Update Start: %s\n", filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    }

    // Write the received bytes
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }

    if (final) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u bytes\n", index + len);
      } else {
        Update.printError(Serial);
      }
      request->send(200, "text/plain", Update.hasError() ? "FAIL" : "OK");
      delay(100);
      ESP.restart();
    }
  }
);
}

ESP2SOTAClass ESP2SOTA;
