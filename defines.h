/*  
Arduino version: 1.8.18
Board information: ESP32 DEV MODUL
Library neoPixel: 1,2,0
https://dl.espressif.com/dl/package_esp32_index.json
*/

#define swap_(a,b) {int16_t t=a;a=b;b=t;}

#define BTN_TOUCH_PIN_1 5
#define BTN_TOUCH_PIN_2 17

#define SDA_PIN_AHT10 33
#define SCL_PIN_AHT10 32

#define LED_PIN_WS2812 27
#define LED_STRIP_LENGHT 14                    
#define LED_STRIP_HIGH 41                   
#define LED_COUNT_LED (LED_STRIP_LENGHT * LED_STRIP_HIGH) 

#define BUZZER_PIN 14

#define SYSTEM_CONFIG_LOOP_UPDATE_mS 1000
#define SYSTEM_CONFIG_LOOP_UPDATE_FIRE_mS 400
#define SYSTEM_CONFIG_DISPLAY_UPDATE_S 10

#define SYSTEM_CONFIG_WIFI_RECONNECT_S 720 

#define CONFIG_TIME_DAY_START 6 //Hour
#define CONFIG_TIME_DAY_END 23 //Hour

#define CONFIG_UPDATE_WEATHER_S 900

#define CONFIG_COLOR_TIME_COLON 255, 255, 0 //RGB
#define CONFIG_COLOR_TIME_COLON_BLINK 255, 255, 255 //RGB

#define CONFIG_LED_SPEED_STEPS 250  //30  //0-255 Light speed_step animation numbers
#define CONFIG_LED_DELAY_STEPS_mS 2         //30       //mS Light delay animation numbers

#define WORK_ROW_OFFSET 6
#define WORK_ROW_1 1
#define WORK_ROW_2 (WORK_ROW_1 + WORK_ROW_OFFSET)
#define WORK_ROW_3 (WORK_ROW_1 + (WORK_ROW_OFFSET * 2))
#define WORK_ROW_4 (WORK_ROW_1 + (WORK_ROW_OFFSET * 3))
#define WORK_ROW_5 (WORK_ROW_1 + (WORK_ROW_OFFSET * 4))
#define WORK_ROW_6 (WORK_ROW_1 + (WORK_ROW_OFFSET * 5))
#define WORK_ROW_7 (WORK_ROW_1 + (WORK_ROW_OFFSET * 6))

#define WORK_COLUMN_OFFSET 4
#define WORK_COLUMN_1 1  // "x" x.x
#define WORK_COLUMN_2 WORK_COLUMN_OFFSET  // x "x".x
#define WORK_COLUMN_3 (WORK_COLUMN_OFFSET * 2)  // x  x ."x"

enum DIPLAY_MODE{
  MODE_NORMAL,
  MODE_FIRE,
  MODE_SNAKE
};

enum COLORS_ENUM{
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE
};

struct esp {
  double temp;
  int humidity;
};