#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "forecast_record.h"
#include <JsonConfig.h>

#define SDCARD_SS 13
#define SDCARD_CLK 14
#define SDCARD_MOSI 15
#define SDCARD_MISO 2

#define MAX_READINGS 7        // Max hourly forecast readings
#define SLEEP_DURATION_MIN 30 // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
#define WAKEUP_TIME_HR 7      // Don't wakeup until after this hour to save battery power
#define SLEEP_TIME_HR 23      // Sleep after this hour to save battery power

#define SHOW_PERCENT_VOLTAGE true

#define VOLTAGE_DIVIDER_RATIO 0.00190972 // Varies from board to board. Equals to 1/(Rd * Vc), where Rd is voltage divider ration (usually 0.5) and Vc is counts per volt (3.3/4096) 

#define METRIC "metric"

const String SSID = F("ssid");
const String PASSWORD = F("password");
const String API_KEY = F("apikey");
const String API_URL = F("server");
const String CITY = F("city");
const String LATITUDE = F("latitude");
const String LONGITUDE = F("longitude");
const String COUNTRY = F("country");
const String LANG = F("language");
const String UNITS = F("units");
const String TIMEZONE = F("timezone");
const String NTP_URL = F("ntpserver");
const String TELEGRAM_TOKEN = F("tg_token");
const String TELEGRAM_CHAT_ID = F("tg_chat_id");

String keys[] = {
    SSID,
    PASSWORD,
    API_KEY,
    API_URL,
    CITY,
    LATITUDE,
    LONGITUDE,
    COUNTRY,
    LANG,
    UNITS,
    TIMEZONE,
    NTP_URL,
    TELEGRAM_TOKEN,
    TELEGRAM_CHAT_ID};


auto configPtr = createJsonConfig(keys);
auto config = *configPtr;

// #define TEST_DATA

ForecastRecord WxConditions[1];
ForecastRecord WxForecast[MAX_READINGS];

String time_str;
String date_str; // strings to hold time and date

int CurrentHour = 0;
int CurrentMin = 0;
int CurrentSec = 0;

long StartTime = 0;

enum RequestType
{
  WEATHER,
  FORECAST
};

bool isMetric()
{
  return config[UNITS] == METRIC;
}

float mm_to_inches(float value_mm)
{
  return 0.0393701 * value_mm;
}

float hPa_to_inHg(float value_hPa)
{
  return 0.750063755419211 * value_hPa;
}

float convert_pressure(float value_hPa)
{
  if (isMetric())
  {
    return value_hPa;
  }
  return hPa_to_inHg(value_hPa);
}

float convert_mm(float value_mm)
{
  if (isMetric())
  {
    return value_mm;
  }
  return mm_to_inches(value_mm);
}

String wind_deg_to_direction(float winddirection)
{
  if (winddirection > 360)
  {
    winddirection -= 360;
  }
  else if (winddirection < 0)
  {
    winddirection += 360;
  }
  if (winddirection >= 348.75 || winddirection < 11.25)
    return TXT_N;
  if (winddirection >= 11.25 && winddirection < 33.75)
    return TXT_NNE;
  if (winddirection >= 33.75 && winddirection < 56.25)
    return TXT_NE;
  if (winddirection >= 56.25 && winddirection < 78.75)
    return TXT_ENE;
  if (winddirection >= 78.75 && winddirection < 101.25)
    return TXT_E;
  if (winddirection >= 101.25 && winddirection < 123.75)
    return TXT_ESE;
  if (winddirection >= 123.75 && winddirection < 146.25)
    return TXT_SE;
  if (winddirection >= 146.25 && winddirection < 168.75)
    return TXT_SSE;
  if (winddirection >= 168.75 && winddirection < 191.25)
    return TXT_S;
  if (winddirection >= 191.25 && winddirection < 213.75)
    return TXT_SSW;
  if (winddirection >= 213.75 && winddirection < 236.25)
    return TXT_SW;
  if (winddirection >= 236.25 && winddirection < 258.75)
    return TXT_WSW;
  if (winddirection >= 258.75 && winddirection < 281.25)
    return TXT_W;
  if (winddirection >= 281.25 && winddirection < 303.75)
    return TXT_WNW;
  if (winddirection >= 303.75 && winddirection < 326.25)
    return TXT_NW;
  if (winddirection >= 326.25 && winddirection < 348.75)
    return TXT_NNW;
  return "?";
}

#endif