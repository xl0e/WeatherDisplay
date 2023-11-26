#include <Arduino.h>
#include <FileUtils.h>
#include <Wire.h>
#include <WiFiClientSecure.h>

#define DEBUG_ON 1

#ifdef DEBUG_ON
#define PRINTLN(...) Serial.println(__VA_ARGS__);
#define PRINT(...) Serial.print(__VA_ARGS__);
#define PRINTF(f, ...) Serial.print(f, __VA_ARGS__);
#else
#define PRINTLN(...) ;
#define PRINT(...) ;
#define PRINTF(f, ...) ;
#endif

#define FILESYSTEM SPIFFS

#include "lang_ru.h"

#include "common.h"

#include "time.h"
#include "config.h"

#include "api/openweathermap/api_functions.h"
#include "draw_functions.h"
#include "system.h"


void begin_sleep();
bool update_local_time();
bool setup_time();

WiFiClientSecure wifiClientSecure;
WiFiClient wifiClient;


void setup()
{
  StartTime = millis();
#ifdef DEBUG_ON
  Serial.begin(115200);
#endif
  PRINTLN("INFO: Serial is started");
  setup_pins();

  led_on();
  enable_flash_memory();
  display_init(); // Give screen time to display_init by getting weather data!
  setup_sd();
  File file = SD.open("/wifi_config.txt", FILE_READ);
  config.init(&file);
  if (start_wifi() == WL_CONNECTED)
  {
    wifiClientSecure.setInsecure();
    if (setup_time() == true)
    {
      //if ((CurrentHour >= WAKEUP_TIME_HR && CurrentHour <= SLEEP_TIME_HR)) {
      byte Attempts = 1;
      bool hasWeatherData = false, hasForecastData = false;
      while ((!hasWeatherData || !hasForecastData) && Attempts <= 5)
      { // Try up-to 2 time for Weather and Forecast data
        if (!hasWeatherData)
          hasWeatherData = API::obtain_wx_data(wifiClient, WEATHER);
        if (!hasForecastData)
          hasForecastData = API::obtain_wx_data(wifiClient, FORECAST);
        Attempts++;
      }
      if (hasWeatherData && hasForecastData)
      {              // Only if received both Weather or Forecast proceed
        
        update_local_time();
        draw_weather();
        stop_wifi(); // Reduces power consumption
      }
      else
      {
        draw_error_msg("Failed to obtain weather data");
      }
    }
    else
    {
      draw_error_msg("Failed to setup time");
    }
  }
  else
  {
    draw_error_msg("Can't connect to Wi-Fi");
  }
  display_update();
  led_off();
  disable_flash_memory();

  begin_sleep();
}

void loop()
{ // this will never run!
}