#ifndef ESP32_APP__SAMPLE_DATA_REQUEST
#define ESP32_APP__SAMPLE_DATA_REQUEST


#include <HTTPClient.h>

String serverName = "https://raw.githubusercontent.com";


void getSampleData()
{
    
    HTTPClient http;

      String serverPath = serverName + "/nemesa/StaticFiles/refs/heads/master/sample.json";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
}

#endif