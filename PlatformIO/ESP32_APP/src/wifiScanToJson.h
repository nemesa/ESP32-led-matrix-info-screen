
#include <ArduinoJson.h>

boolean WIFI_SCAN_REQUESTED = false;
JsonDocument wifiScanDoc;

const int maxNetworks = 50; // Adjust this value based on your needs

void clearWifiScanResultAndDoNewScan()
{
  wifiScanDoc.clear();
  WIFI_SCAN_REQUESTED = true;
}

// Function to return encryption type as a string
const char *getEncryptionType(wifi_auth_mode_t encryptionType)
{
  switch (encryptionType)
  {
  case WIFI_AUTH_OPEN:
    return "None";
  case WIFI_AUTH_WEP:
    return "WEP";
  case WIFI_AUTH_WPA_PSK:
    return "WPA/PSK";
  case WIFI_AUTH_WPA2_PSK:
    return "WPA2/PSK";
  case WIFI_AUTH_WPA_WPA2_PSK:
    return "WPA/WPA2/PSK";
  case WIFI_AUTH_WPA2_ENTERPRISE:
    return "WPA2 Enterprise";
  case WIFI_AUTH_WPA3_PSK:
    return "WPA3/PSK";
  default:
    return "Unknown";
  }
}

// Function to scan Wi-Fi networks and return a JSON-formatted string
void scanWiFiNetworks()
{
  // Start scanning
  int n = WiFi.scanNetworks();
  wifiScanDoc.clear();
  
  // Create a JSON array to hold network information
  JsonArray networkArray = wifiScanDoc["networks"].to<JsonArray>();

  // If networks are found, process them
  if (n == 0)
  {
    JsonObject obj = networkArray.add<JsonObject>();
    obj["SSID"] = "No networks found";
    obj["RSSI"] = "N/A";
    obj["EncryptionType"] = "N/A";
  }
  else
  {
    for (int i = 0; i < n && i < maxNetworks; i++)
    {
      JsonObject obj = networkArray.add<JsonObject>();
      obj["SSID"] = WiFi.SSID(i);                                        // SSID of the network
      obj["RSSI"] = WiFi.RSSI(i);                                        // Signal strength
      obj["EncryptionType"] = getEncryptionType(WiFi.encryptionType(i)); // Encryption type
    }
  }
}

void wifiScanLoop()
{  
  if (WIFI_SCAN_REQUESTED)
  {
    scanWiFiNetworks();
    WIFI_SCAN_REQUESTED = false;
  }
}