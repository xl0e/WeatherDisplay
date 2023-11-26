#ifndef LANG_H_
#define LANG_H_

#define FONT(x) x##_tf

const String TXT_NL         = F("\n");
const String TXT_SPACE         = F(" ");

const String TXT_FORECAST   = F("Forecast");
const String TXT_PRESSURE   = F("Pressure");
const String TXT_RAIN       = F("Rain");
const String TXT_SNOW       = F("Snow");

const String TXT_KPA        = F("kPa");
const String TXT_MM_HG      = F("mmHg");
const String TXT_M_S        = F("m/s");
const String TXT_MM         = F("mm");


//Wind
const String TXT_N   = F("N");
const String TXT_NNE = F("NNE");
const String TXT_NE  = F("NE");
const String TXT_ENE = F("ENE");
const String TXT_E   = F("E");
const String TXT_ESE = F("ESE");
const String TXT_SE  = F("SE");
const String TXT_SSE = F("SSE");
const String TXT_S   = F("S");
const String TXT_SSW = F("SSW");
const String TXT_SW  = F("SW");
const String TXT_WSW = F("WSW");
const String TXT_W   = F("W");
const String TXT_WNW = F("WNW");
const String TXT_NW  = F("NW");
const String TXT_NNW = F("NNW");

//Day of the week
const char* weekday_D[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//Month
const char* month_M[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

#endif //LANG_H_