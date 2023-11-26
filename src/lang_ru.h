#ifndef LANG_H_
#define LANG_H_

#define FONT(x) x##_tf

const String TXT_NL         = F("\n");
const String TXT_SPACE         = F(" ");

const String TXT_FORECAST   = F("Прогноз");
const String TXT_PRESSURE   = F("Давление");
const String TXT_RAIN       = F("Дождь");
const String TXT_SNOW       = F("Снег");

const String TXT_KPA        = F("кПа");
const String TXT_MM_HG      = F("мм.рт.ст.");
const String TXT_M_S        = F("м/с");
const String TXT_MM         = F("мм");


//Wind
const String TXT_N   = F("С");
const String TXT_NNE = F("ССВ");
const String TXT_NE  = F("СВ");
const String TXT_ENE = F("ЗСЗ");
const String TXT_E   = F("В");
const String TXT_ESE = F("ВЮВ");
const String TXT_SE  = F("ЮВ");
const String TXT_SSE = F("ББВ");
const String TXT_S   = F("Ю");
const String TXT_SSW = F("ЮЮЗ");
const String TXT_SW  = F("ЮЗ");
const String TXT_WSW = F("ЗЮЗ");
const String TXT_W   = F("З");
const String TXT_WNW = F("ЗСЗ");
const String TXT_NW  = F("СЗ");
const String TXT_NNW = F("ССЗ");

//Day of the week
const char* weekday_D[] = { "Вск", "Пнд", "Втр", "Срд", "Чтв", "Птн", "Сбт" };

//Month
const char* month_M[] = { "Янв", "Фев", "Мар", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек" };

#endif //LANG_H_