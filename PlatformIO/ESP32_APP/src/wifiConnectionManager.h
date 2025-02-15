
#include "settingsHandler.h"
#include <WiFi.h>

#include <ArduinoJson.h>

boolean WIFI_CONNECTION_MANAGER_TRY_CONNECT = false;
boolean WIFI_CONNECTION_TRY_RECONNECT = false;
boolean WIFI_CONNECTION_HAS_INTERNET_ACCESS = false;

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void wifiManagerInit()
{
    WiFi.mode(WIFI_AP_STA);
}

void wifiManagerCreateAP()
{

    settingsJson = readSettingsJson();

    const char *ap_ssid = settingsJson["ap_ssid"];
    // Connect to WiFi network

    if (settingsJson["ap_password"])
    {
        const char *ap_password = settingsJson["ap_password"];
        WiFi.softAP(ap_ssid, ap_password);
    }
    else
    {
        WiFi.softAP(ap_ssid);
    }

    WiFi.softAPConfig(local_ip, gateway, subnet);

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void wifiManagerConnectWifiBasedOnConfig()
{
    WIFI_CONNECTION_HAS_INTERNET_ACCESS = false;
    Serial.println("wifiManagerConnectWifiBasedOnConfig");
    settingsJson = readSettingsJson();
    if (settingsJson["connect_to_ssid"])
    {
        const char *connect_to_ssid = settingsJson["connect_to_ssid"];

        if (WiFi.status() == WL_CONNECTED)
        {
            const char *actualWifi = WiFi.SSID().c_str();
            if (strcmp(actualWifi, connect_to_ssid) == 0)
            {
                Serial.println("Already connected to the same WiFi");
                return;
            }
            else
            {
                Serial.println("Disconnecting from current WiFi");
                WIFI_CONNECTION_TRY_RECONNECT = false;
                WiFi.disconnect();
            }
        }

        if (settingsJson["connect_to_password"])
        {
            const char *connect_to_password = settingsJson["connect_to_password"];
            WiFi.begin(connect_to_ssid, connect_to_password);
        }
        else
        {
            WiFi.begin(connect_to_ssid);
        }

        Serial.print("Connecting to ");
        Serial.println(connect_to_ssid);

        int i = 0;
        while (WiFi.status() != WL_CONNECTED && i < 20)
        {
            delay(1000);
            Serial.println(++i);
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Connected to the WiFi network");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            WIFI_CONNECTION_TRY_RECONNECT = true;
            WIFI_CONNECTION_HAS_INTERNET_ACCESS = true;
        }
        else
        {
            Serial.println("Failed to connect to the WiFi network");
        }
    }
    else
    {
        Serial.println("No WiFi to connect to! (in config file)");
    }
}

void wifiManagerDisconnectWifi()
{
    Serial.println("Disconnecting from WiFi");
    WIFI_CONNECTION_TRY_RECONNECT = false;
    WIFI_CONNECTION_MANAGER_TRY_CONNECT = false;
    WIFI_CONNECTION_HAS_INTERNET_ACCESS = false;
    WiFi.disconnect();
}

JsonDocument wifiManagerConnectionStatus()
{
    JsonDocument doc;
    doc["status"] = WiFi.status();
    if (WiFi.status() == WL_CONNECTED)
    {
        doc["ip"] = WiFi.localIP().toString();
        doc["rssi"] = WiFi.RSSI();
        WIFI_CONNECTION_HAS_INTERNET_ACCESS = true;
    }
    else
    {
        doc["ip"] = (char *)NULL;
        doc["rssi"] = (char *)NULL;
        WIFI_CONNECTION_HAS_INTERNET_ACCESS = false;
    }

    return doc;
}

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void wifiManagerLoop()
{
    if ((WIFI_CONNECTION_TRY_RECONNECT))
    {
        unsigned long currentMillis = millis();
        if (WiFi.status() != WL_CONNECTED && (currentMillis - previousMillis >= interval))
        {
            previousMillis = millis();
            Serial.println("Trying to reconnect to WiFi");
            WiFi.reconnect();
        }
    }

    if (WIFI_CONNECTION_MANAGER_TRY_CONNECT)
    {
        wifiManagerConnectWifiBasedOnConfig();
        WIFI_CONNECTION_MANAGER_TRY_CONNECT = false;
    }
}