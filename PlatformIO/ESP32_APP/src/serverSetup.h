#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "dateTimeHandler.h"
// https://github.com/ESP32Async/ESPAsyncWebServer#send-large-webpage-from-progmem
AsyncWebServer server(80);

const char app_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
<head>
</head>
<body>    
    <div>
        hello from app
    </div>
</body>
</html>
)rawliteral";

void setupServer()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /");        
    request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/ko.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /ko.js");
    request->send(SPIFFS, "/ko.3.5.1.min.js", "text/javascript"); });

  server.on("/toast.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /toast.js");
    request->send(SPIFFS, "/toastify-js.1.12.0.min.js", "text/javascript"); });

  server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /scan-wifi");
    clearWifiScanResultAndDoNewScan();
    request->send(200, "text/plain", "OK"); });

  server.on("/scan-wifi-result", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /scan-wifi-result");
    // Create a char array to store the JSON string
    static char jsonOutput[1024];
    serializeJson(wifiScanDoc, jsonOutput);
  
    request->send(200, "application/json", jsonOutput); });

  server.on("/connection-info", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /connection-info");

    static char jsonOutput[1024];
    serializeJson(wifiManagerConnectionStatus(), jsonOutput);
  
        getSystemISOTimeString();
    request->send(200, "application/json", jsonOutput); });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("HTTP_GET /settings");
    static char jsonOutput[1024];
    serializeJson(settingsJson, jsonOutput);
  
    request->send(200, "application/json", jsonOutput); });

  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                       {
    Serial.printf("onRequestBody");
    Serial.println(request->url().c_str());

    if(!index){
      JsonDocument doc;
      deserializeJson(doc, data);

      if (request->url() == "/settings"){
        Serial.println("HTTP_POST /settings");          
        updateSettings(doc);
        if(doc["connect_to_ssid"]){
          WIFI_CONNECTION_MANAGER_TRY_CONNECT = true;
        }
        else{              
          wifiManagerDisconnectWifi();
        }
      }
      else if (request->url() == "/connect-wifi"){
        Serial.println("HTTP_POST /connect-wifi");        
        if(doc["password"]){
          setConnectedWifiInSettings(doc["ssid"], doc["password"]);
        }
        else{
          setConnectedWifiInSettings(doc["ssid"]);
        }
        WIFI_CONNECTION_MANAGER_TRY_CONNECT = true;        
      }
    }

    request->send(200); });
}
