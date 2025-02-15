#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "wifiConnectionManager.h"
#include "wifiScanToJson.h"
#include "serverSetup.h"
#include "ledMatrix.h"

#include "sampleDataRequest.h"

#define LED_PIN 2 // In some ESP32 board have inbuilt LED

volatile int interruptCounter; // for counting interrupt
int totalInterruptCounter;     // total interrupt counting
int LED_STATE = LOW;
hw_timer_t *timer = NULL; // H/W timer defining (Pointer to the Structure)

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer()
{ // Defining Inerrupt function with IRAM_ATTR for faster access
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

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

  pinMode(LED_PIN, OUTPUT);

  timer = timerBegin(0, 80, true);             // timer 0, prescalar: 80, UP counting
  timerAttachInterrupt(timer, &onTimer, true); // Attach interrupt
  timerAlarmWrite(timer, 1000000, true);       // Match value= 1000000 for 1 sec. delay.
  timerAlarmEnable(timer);                     // Enable Timer with interrupt (Alarm Enable)
}

void loop() {
   wifiScanLoop();
   wifiManagerLoop();
 if (interruptCounter > 0) {
 
   portENTER_CRITICAL(&timerMux);
   interruptCounter--;
   portEXIT_CRITICAL(&timerMux);
 
   totalInterruptCounter++;         	//counting total interrupt

   LED_STATE= !LED_STATE;  				//toggle logic
   digitalWrite(LED_PIN, LED_STATE);    //Toggle LED
   Serial.print("An interrupt as occurred. Total number: ");
   Serial.println(totalInterruptCounter);
   Serial.println(getSystemISOTimeString());

   led_matrix_clearRectangle(1, 35, 128, 8);
   led_matrix_write_str(1, 35, getSystemISOTimeString());
 }
}