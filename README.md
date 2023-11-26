# WeatherDisplay
## How to upload firmaware to the board
1. Open the project in VS Code
2. Check the display.h and uncomment the relevant display version driver.
3. Open the config_sample/wifi_config.txt and provide valid settings for your wi-fi and region. Copy it to any Micro SD card. 
4. Upload firmware to the board and insert the card into the board. 
5. Check the serial logs for any errors. Find the line with voltage readings e.g. 'INFO: Voltage = 4.20'. If value doesn't correspond to the measured source voltage update VOLTAGE_DIVIDER_RATIO in common.h. 
