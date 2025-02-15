#ifndef ESP32_APP__DATE_TIME_HANDLER_H
#define ESP32_APP__DATE_TIME_HANDLER_H

#include <Arduino.h>
#include "time.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0; // 3600;

void configTimeHandler()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

char timeBuffer[25];

char *getTimeServerISOTimeString(uint32_t timeout_ms)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, timeout_ms))
  {
    Serial.println("Failed to obtain time");
    timeBuffer[0] = '1';
    timeBuffer[1] = '9';
    timeBuffer[2] = '0';
    timeBuffer[3] = '0';
    timeBuffer[4] = '-';
    timeBuffer[5] = '0';
    timeBuffer[6] = '1';
    timeBuffer[7] = '-';
    timeBuffer[8] = '0';
    timeBuffer[9] = '1';
    timeBuffer[10] = 'T';
    timeBuffer[11] = '0';
    timeBuffer[12] = '0';
    timeBuffer[13] = ':';
    timeBuffer[14] = '0';
    timeBuffer[15] = '0';
    timeBuffer[16] = ':';
    timeBuffer[17] = '0';
    timeBuffer[18] = '0';
    timeBuffer[19] = '.';
    timeBuffer[20] = '0';
    timeBuffer[21] = '0';
    timeBuffer[22] = '0';
    timeBuffer[23] = 'Z';
    timeBuffer[24] = ' ';
  }
  else
  {
    strftime(timeBuffer, 25, "%FT%H:%M:%S.000Z", &timeinfo);
  }

  return timeBuffer;
}

char *getSystemISOTimeString()
{
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(timeBuffer, 25, "%FT%H:%M:%S.000Z", timeinfo);

  return timeBuffer;
}

#endif