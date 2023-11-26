#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <WiFi.h>

#include "common.h"
#include "display.h"

#define FLASH_CS_PIN 11
#define BUILTIN_LED_PIN 19
#define VOLTAGE_PIN 35

SPIClass sdSPI(VSPI);

void setup_pins()
{
  PRINT("INFO: Setup pins... ");
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(FLASH_CS_PIN, OUTPUT);
  pinMode(VOLTAGE_PIN, INPUT);
  PRINTLN("OK");
}
void setup_sd()
{
  PRINTLN("Setup SD card...");
  sdSPI.begin(SDCARD_CLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_SS);
  SD.begin(SDCARD_SS, sdSPI);
}

void led_on()
{
  digitalWrite(BUILTIN_LED_PIN, HIGH);
}

void led_off()
{
  digitalWrite(BUILTIN_LED_PIN, LOW);
}

void enable_flash_memory()
{
  digitalWrite(FLASH_CS_PIN, LOW);
}

void disable_flash_memory()
{
  digitalWrite(FLASH_CS_PIN, HIGH);
}

uint8_t start_wifi()
{
  PRINT("INFO: Connecting to Wi-Fi... ");
  PRINT(config[SSID]);
  PRINT(":");
  PRINTLN(config[PASSWORD]);
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(config[SSID].c_str(), config[PASSWORD].c_str());
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection)
  {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000)
    { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED)
    {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED)
  {
    int wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    PRINT("INFO: WiFi strength - ");
    PRINTLN(wifi_signal);
    PRINTLN("INFO: WiFi connected at: " + WiFi.localIP().toString());
  }
  else
    PRINTLN("ERROR: WiFi connection *** FAILED ***");
  return connectionStatus;
}

void stop_wifi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void begin_sleep()
{
  display_power_off();
  long SleepTimer = (SLEEP_DURATION_MIN * 60 - ((CurrentMin % SLEEP_DURATION_MIN) * 60 + CurrentSec)); // Some ESP32 are too fast to maintain accurate time
  esp_sleep_enable_timer_wakeup((SleepTimer + 20) * 1000000LL);                                        // Added 20-sec extra delay to cater for slow ESP32 RTC timers

  PRINTLN("INFO: Entering " + String(SleepTimer) + "-secs of sleep time");
  PRINTLN("INFO: Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
  PRINTLN("INFO: Starting deep-sleep period...");

  gpio_deep_sleep_hold_en();
  esp_deep_sleep_start(); // Sleep for e.g. 30 minutes
}

bool update_local_time()
{
  PRINT("INFO: Updating local time... ");
  struct tm timeinfo;
  char time_output[30], day_output[30], update_time[30];
  while (!getLocalTime(&timeinfo, 5000))
  { // Wait for 5-sec for time to synchronise
    PRINTLN("ERROR: Failed to obtain time");
    return false;
  }
  CurrentHour = timeinfo.tm_hour;
  CurrentMin = timeinfo.tm_min;
  CurrentSec = timeinfo.tm_sec;
  // See http://www.cplusplus.com/reference/ctime/strftime/
  PRINT(&timeinfo, "%a %b %d %Y %H:%M"); // Displays: Saturday, June 24 2017 14:05:49
  if (config[UNITS] == "metric")
  {
    if ((config[LANG] == "CZ") || (config[LANG] == "DE") || (config[LANG] == "NL") || (config[LANG] == "PL") || (config[LANG] == "GR"))
    {
      sprintf(day_output, "%s, %02u. %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s  %02u-%s-%04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M", &timeinfo); // Creates: '@ 14:05:49'   and change from 30 to 8 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sprintf(time_output, "%s", update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);        // Creates: '@ 02:05:49pm'
    sprintf(time_output, "%s", update_time);
  }
  date_str = day_output;
  time_str = time_output;
  PRINTLN(" DONE");
  return true;
}

bool setup_time()
{
  PRINT("INFO: Setup time... ");
  configTime(0, 3600, config[NTP_URL].c_str(), "time.nist.gov"); //(gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", config[TIMEZONE].c_str(), 1);                     // setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  tzset();                                                       // Set the TZ environment variable
  delay(100);
  PRINTLN("DONE");
  return update_local_time();
}

float read_battery_voltage()
{
  PRINT("INFO: Read battery voltage... ");
  float voltageRaw = 0;
  for (int i = 0; i < 10; i++)
  {
    voltageRaw += analogRead(VOLTAGE_PIN);
    delay(10);
  }
  voltageRaw /= 10;
  PRINTLN("DONE");
  return voltageRaw;
}

#endif // SYSTEM_H_