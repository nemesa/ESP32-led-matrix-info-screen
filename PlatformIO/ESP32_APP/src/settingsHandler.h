#include "SPIFFS.h"
#include <ArduinoJson.h>

JsonDocument settingsJson; 

JsonDocument readSettingsJson()
{
    JsonDocument doc;    
    Serial.println(F("open settings file"));
    
    File file = SPIFFS.open("/settings.json");
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println(F("Failed to read settings.json"));
    }    
    file.close();

    return doc;
}

void writeSettingsJson(JsonDocument doc)
{
    Serial.println(F("write settings file"));
    File file = SPIFFS.open("/settings.json", FILE_WRITE);
    serializeJson(doc, file);
    file.close();
}

void updateSettings(JsonDocument doc)
{
    settingsJson["ap_ssid"] = doc["ap_ssid"];
    settingsJson["ap_password"] = doc["ap_password"];
    settingsJson["connect_to_ssid"] = doc["connect_to_ssid"];
    settingsJson["connect_to_password"] = doc["connect_to_password"];
    writeSettingsJson(settingsJson);
}

void setConnectedWifiInSettings(const char *ssid)
{
    settingsJson["connect_to_ssid"] = ssid;
    settingsJson["connect_to_password"] = (char*)NULL;

    writeSettingsJson(settingsJson);
}

void setConnectedWifiInSettings(const char *ssid, const char *password)
{
    settingsJson["connect_to_ssid"] = ssid;
    settingsJson["connect_to_password"] = password;

    writeSettingsJson(settingsJson);
}
