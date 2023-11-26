#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "common.h"

namespace API
{

  String request_type_to_string(const RequestType type)
  {
    return type == WEATHER ? "weather" : "forecast";
  }

  // Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
  bool decode_weather(WiFiClient &json, const RequestType type)
  {
    // allocate the JsonDocument
    DynamicJsonDocument doc(35 * 1024);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);
    // Test if parsing succeeds.
    if (error)
    {
      PRINT(F("ERROR: JSON deserialization failed: "));
      PRINTLN(error.c_str());
      return false;
    }
    // convert it to a JsonObject
    JsonObject root = doc.as<JsonObject>();
    PRINTLN("INFO: Decoded [" + request_type_to_string(type) + "] data:");
    if (type == WEATHER)
    {
      // All PRINTLN statements are for diagnostic purposes and not required, remove if not needed
      WxConditions[0].icon = root["weather"][0]["icon"].as<const char *>();
      PRINTLN("Icon: " + String(WxConditions[0].icon));
      WxConditions[0].temperature = root["main"]["temp"].as<float>();
      PRINTLN("Temp: " + String(WxConditions[0].temperature));
      WxConditions[0].pressure = root["main"]["pressure"].as<float>();
      PRINTLN("Pres: " + String(WxConditions[0].pressure));
      WxConditions[0].humidity = root["main"]["humidity"].as<float>();
      PRINTLN("Humi: " + String(WxConditions[0].humidity));
      WxConditions[0].tempMin = root["main"]["temp_min"].as<float>();
      PRINTLN("TLow: " + String(WxConditions[0].tempMin));
      WxConditions[0].tempMax = root["main"]["temp_max"].as<float>();
      PRINTLN("THig: " + String(WxConditions[0].tempMax));
      WxConditions[0].windSpeed = root["wind"]["speed"].as<float>();
      PRINTLN("WSpd: " + String(WxConditions[0].windSpeed));
      WxConditions[0].windDirection = root["wind"]["deg"].as<float>();
      PRINTLN("WDir: " + String(WxConditions[0].windDirection));
      WxConditions[0].cloudCover = root["clouds"]["all"].as<int>();
      PRINTLN("CCov: " + String(WxConditions[0].cloudCover)); // in % of cloud cover
      WxConditions[0].rainfall = root["rain"]["1h"].as<float>();
      PRINTLN("Rain: " + String(WxConditions[0].rainfall));
      WxConditions[0].snowfall = root["snow"]["1h"].as<float>();
      PRINTLN("Snow: " + String(WxConditions[0].snowfall));
    }
    if (type == FORECAST)
    {
      PRINT(F("INFO: Receiving Forecast period - "));
      JsonArray list = root["list"];
      for (byte r = 0; r < MAX_READINGS; r++)
      {
        PRINTLN("\nPeriod-" + String(r));
        WxForecast[r].timestamp = list[r]["dt"].as<int>();
        PRINTLN("DTim: " + String(WxForecast[r].timestamp));
        WxForecast[r].dateTime = list[r]["dt_txt"].as<const char *>();
        PRINTLN("Date: " + String(WxForecast[r].dateTime));
        WxForecast[r].temperature = list[r]["main"]["temp"].as<float>();
        PRINTLN("Temp: " + String(WxForecast[r].temperature));
        WxForecast[r].tempMin = list[r]["main"]["temp_min"].as<float>();
        PRINTLN("TLow: " + String(WxForecast[r].tempMin));
        WxForecast[r].tempMax = list[r]["main"]["temp_max"].as<float>();
        PRINTLN("THig: " + String(WxForecast[r].tempMax));
        WxForecast[r].pressure = convert_pressure(list[r]["main"]["pressure"].as<float>());
        PRINTLN("Pres: " + String(WxForecast[r].pressure));
        WxForecast[r].humidity = list[r]["main"]["humidity"].as<float>();
        PRINTLN("Humi: " + String(WxForecast[r].humidity));
        WxForecast[r].icon = list[r]["weather"][0]["icon"].as<const char *>();
        PRINTLN("Icon: " + String(WxForecast[r].icon));
        WxForecast[r].description = list[r]["weather"][0]["description"].as<const char *>();
        PRINTLN("Desc: " + String(WxForecast[r].description));
        WxForecast[r].cloudCover = list[r]["clouds"]["all"].as<int>();
        PRINTLN("CCov: " + String(WxForecast[r].cloudCover)); // in % of cloud cover
        WxForecast[r].windSpeed = list[r]["wind"]["speed"].as<float>();
        PRINTLN("WSpd: " + String(WxForecast[r].windSpeed));
        WxForecast[r].windDirection = list[r]["wind"]["deg"].as<float>();
        PRINTLN("WDir: " + String(WxForecast[r].windDirection));
        WxForecast[r].rainfall = convert_mm(list[r]["rain"]["3h"].as<float>());
        PRINTLN("Rain: " + String(WxForecast[r].rainfall));
        WxForecast[r].snowfall = convert_mm(list[r]["snow"]["3h"].as<float>());
        PRINTLN("Snow: " + String(WxForecast[r].snowfall));
        WxForecast[r].pop = list[r]["pop"].as<float>();
        PRINTLN("Pop:  " + String(WxForecast[r].pop));
      }
      float pressure_trend = WxForecast[0].pressure - WxForecast[2].pressure; // Measure pressure slope between ~now and later
      pressure_trend = ((int)(pressure_trend * 10)) / 10.0;                   // Remove any small variations less than 0.1
      WxConditions[0].pressureTrend = "0";
      if (pressure_trend > 0)
        WxConditions[0].pressureTrend = "+";
      if (pressure_trend < 0)
        WxConditions[0].pressureTrend = "-";
      if (pressure_trend == 0)
        WxConditions[0].pressureTrend = "0";
    }
    return true;
  }

  String build_location()
  {
    return config[LATITUDE] == "" || config[LONGITUDE] == ""
               ? "?q=" + config[CITY] + "," + config[COUNTRY]
               : "?lat=" + config[LATITUDE] + "&lon=" + config[LONGITUDE];
  }

  String build_request_url(const RequestType requestType)
  {
    const String units = config[UNITS];
    return "/data/2.5/" + request_type_to_string(requestType) + build_location() + "&APPID=" + config[API_KEY] + "&mode=json&units=" + units + "&lang=" + config[LANG];
  }

  bool obtain_wx_data(WiFiClient &client, const RequestType requestType)
  {
    client.stop(); // close connection before sending a new request
    HTTPClient http;
    String uri = build_request_url(requestType);
    PRINT("INFO: Request URI: ");
    PRINTLN(uri);
    if (requestType == FORECAST)
    {
      uri += "&cnt=" + String(MAX_READINGS);
    }
    //http.begin(uri,test_root_ca); //HTTPS example connection
    http.begin(client, config[API_URL], 80, uri);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      if (!decode_weather(http.getStream(), requestType))
        return false;
      client.stop();
      http.end();
      return true;
    }
    else
    {
      PRINT("ERROR: Connection failed, error: %s");
      PRINTLN(http.errorToString(httpCode).c_str());
      client.stop();
      http.end();
      return false;
    }
    http.end();
    return true;
  }
}
#endif /* ifndef COMMON_H_ */
