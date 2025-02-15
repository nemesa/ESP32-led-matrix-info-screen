#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "wifiConnectionManager.h"
#include "wifiScanToJson.h"
#include "serverSetup.h"
#include "ledMatrix.h"

#include "sampleDataRequest.h"

void setup()
{
  Serial.begin(115200);
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  uint8_t bightness = 20;
  led_matrix_setup(bightness);

  wifiManagerInit();
  Serial.println("Setting up Access Point: ");
  wifiManagerCreateAP();

  Serial.println("Connectiong Wifi if configured: ");
  wifiManagerConnectWifiBasedOnConfig();

  Serial.println("Setting up Async WebServer");
  setupServer();
  server.begin();
  Serial.println("All Done!");
  led_matrix_write(1, 5, "WIFI Setup Done");
  if (WIFI_CONNECTION_HAS_INTERNET_ACCESS)
  {
    led_matrix_write(1, 15, "Sync Time with NTP");

    Serial.println("Configure time server");
    configTimeHandler();

    Serial.println("ISO time:");
    Serial.println(getTimeServerISOTimeString(10000));

    led_matrix_write(1, 25, "Time Setup Done");
    led_matrix_write(1, 35, getSystemISOTimeString());
  }
  else
  {
    led_matrix_write(1, 15, "No internet access...");
  }

  led_matrix_write(1, 55, "READY!");

  getSampleData();
}

void loop()
{
  wifiScanLoop();
  wifiManagerLoop();
  // put your main code here, to run repeatedly:
}
