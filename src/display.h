#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <SPI.h>

// #include <GxEPD2.h>
// #include <GxEPD2_BW.h>

#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
// #include <GxGDEW0213Z16/GxGDEW0213Z16.h>  // 2.13" b/w/r form GoodDisplay
// #include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B72/GxGDEH0213B72.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B73/GxGDEH0213B73.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" b/w  form GoodDisplay 4-color
// #include <GxGDEW0213M21/GxGDEW0213M21.h>  // 2.13" b/w Ultra wide temperature , form GoodDisplay
#include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP

// #include "fonts/arial5pt7b.h"
// #include "fonts/arial6pt7b.h"
// #include "fonts/arial7pt7b.h"
// #include "fonts/arial8pt7b.h"
// #include "fonts/arial9pt7b.h"
// #include "fonts/arial13pt7b.h"

#include "fonts/FreeSans5.h"
#include "fonts/FreeSans6.h"
#include "fonts/FreeSans9.h"
#include "fonts/FreeSans13.h"


#define SCREEN_WIDTH 248
#define SCREEN_HEIGHT 122

enum AlignmentType
{
  LEFT,
  RIGHT,
  CENTER
};

#define EPD_BUSY  4  // to EPD BUSY
#define EPD_CS    5  // to EPD CS
#define EPD_RST  16  // to EPD RST
#define EPD_DC   17  // to EPD DC
#define EPD_SCK  18  // to EPD CLK
#define EPD_MISO -1  // MISO is not used, as no data from display
#define EPD_MOSI 23  // to EPD DIN

// #define DEFALUT_FONT arial6pt7b
#define DEFALUT_FONT FreeSans6pt8b

// #define CITY_FONT arial9pt7b
#define CITY_FONT FreeSans9pt8b
// #define TEMP_FONT arial13pt7b
#define TEMP_FONT FreeSans13pt8b
// #define WEATHER_FONT arial6pt7b
#define WEATHER_FONT FreeSans6pt8b
// #define FORECAST_FONT arial5pt7b
#define FORECAST_FONT FreeSans5pt8b
// #define BIG_FONT arial9pt7b
#define BIG_FONT FreeSans9pt8b
// #define SMALL_FONT arial6pt7b
#define SMALL_FONT FreeSans6pt8b

#define FG_COLOR GxEPD_BLACK
#define BG_COLOR GxEPD_WHITE

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RST);
GxEPD_Class display(io, EPD_RST, EPD_BUSY);

// #define GxEPD2_DISPLAY_CLASS GxEPD2_BW

// // #define GxEPD2_DRIVER_CLASS GxEPD2_213_B72    // GDEH0213B72 128x250
// #define GxEPD2_DRIVER_CLASS GxEPD2_213_B73 // GDEH0213B73 128x250
// #define ENABLE_GxEPD2_GFX 0
// #define GxEPD2_BW_IS_GxEPD2_BW true
// #define IS_GxEPD(c, x) (c##x)
// #define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)

// #define MAX_DISPLAY_BUFFER_SIZE (81920ul-34000ul-34000ul) // ~34000 base use, WiFiClientSecure seems to need about 34k more to work

// #if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
// #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// #endif

// GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));


void display_init()
{
  PRINT("Begin display initialisation ...");
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  display.init(0);
  display.setRotation(1); // Use 1 or 3 for landscape modes
  display.setTextColor(FG_COLOR);
  display.setFont(&DEFALUT_FONT);
  display.fillScreen(BG_COLOR);
  PRINTLN("DONE");
}

void display_power_off()
{
  display.powerDown();
  // display.hibernate();
}

void display_update()
{
  // display.display();
  display.update();
}

#endif
