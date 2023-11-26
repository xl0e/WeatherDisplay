#ifndef FORECAST_RECORD_H_
#define FORECAST_RECORD_H_

#include <Arduino.h>

typedef struct {
  int      timestamp;
  String   dateTime;
  String   icon;
  String   pressureTrend;
  String   description;
  float    temperature;
  float    humidity;
  float    tempMax;
  float    tempMin;
  float    windDirection;
  float    windSpeed;
  float    rainfall;
  float    snowfall;
  float    pop;
  float    pressure;
  int      cloudCover;
} ForecastRecord;

#endif /* ifndef FORECAST_RECORD_H_ */
