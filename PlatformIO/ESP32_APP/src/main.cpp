#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "wifiConnectionManager.h"
#include "wifiScanToJson.h"
#include "serverSetup.h"
#include "ledMatrix.h"
#include "timer.h"

#include "sampleDataRequest.h"

//#define LED_PIN 2 // In some ESP32 board have inbuilt LED
//pinMode(LED_PIN, OUTPUT);

void setup()
{
  Serial.begin(115200);

  settingsHandlerSetup();

  uint8_t bightness = uint8_t(settingsJson["led_matrix_brightness"]);
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
  led_matrix_write_str(1, 5, "WIFI Setup Done");
  if (WIFI_CONNECTION_HAS_INTERNET_ACCESS)
  {
    led_matrix_write_str(1, 15, "Sync Time with NTP");

    Serial.println("Configure time server");
    configTimeHandler();

    Serial.println("ISO time:");
    Serial.println(getTimeServerISOTimeString(10000));

    led_matrix_write_str(1, 25, "Time Setup Done");
    led_matrix_write_str(1, 35, getSystemISOTimeString());
    getSampleData();
  }
  else
  {
    led_matrix_write_str(1, 15, "No internet access...");
  }

  led_matrix_write_str(1, 55, "READY!");

  hw_timer_setup();
}

void loop()
{
  wifiScanLoop();
  wifiManagerLoop();
  hw_timer_loop();
}