#define AHT_SENSOR 1
#define SGP_SENSOR 1

#include "defines.h"
#include "WiFi.h"
#include "time.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHT10.h>
#include <Wire.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp_30;
Adafruit_AHT10 aht;
sensors_event_t aht_10_humidity, aht_10_temperature;

int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel strip(LED_COUNT_LED, LED_PIN_WS2812, NEO_GRB + NEO_KHZ800);

const char* ntpServer = "pool.ntp.org";  // time
long gmtOffset_sec = 3600;               //3600 sommer / 0 winter   ,,,,,,,,,ab 26.03 bis 29.10 - 3600 time
const int daylightOffset_sec = 3600;     //time

struct tm timeinfo;

uint8_t CURRENT_MODE = MODE_NORMAL;

bool day_activated = true;
uint16_t CURRENT_BRIGHTNESS = 0;

const String openWeatherMapApiKey = "467236d17fdfc652af154eb837422f8d";
String city = "Dortmund";
String countryCode = "DE";

String jsonBuffer;

String receivedMessage;

esp esp_data;

uint8_t buf_color[3] = { 255, 255, 255 };

const char* ssid = "WiFi";
const char* password = "Lokomotive132";

/////////////////////////////////////////////////////////////////////////////protho
void push_all_values();

void ini_ws2812b() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(1);  // Set BRIGHTNESS to about 1/5 (max = 255)
}
void draw_error_pixel() {
  draw_char('c', 1, 1, 255, 255, 255);
  draw_char('h', 1, 7, 255, 255, 255);
  draw_char('e', 1, 13, 255, 255, 255);
  draw_char('c', 1, 19, 255, 255, 255);
  draw_char('k', 1, 25, 255, 255, 255);
}
void draw_pixel_raw(const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  uint16_t y_temp = y;
  uint16_t x_temp = x;
  if (y_temp > LED_STRIP_HIGH) {
    draw_error_pixel();
    return;
    y_temp = LED_STRIP_HIGH;
  }
  if (x > LED_STRIP_LENGHT) {
    draw_error_pixel();
    return;
    x_temp = LED_STRIP_LENGHT;
  }
  y_temp = y_temp % 2;  // 0 or 1
  if (y_temp == 0) {
    x_temp = LED_STRIP_LENGHT * y - x_temp;  //y == 4
  }
  if (y_temp == 1) {
    x_temp = x_temp + LED_STRIP_LENGHT * (y - 1) - 1;  //y == 5
  }
  strip.setPixelColor(x_temp, strip.Color(r, g, b));
}
void draw_pixel_now(const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  draw_pixel_raw(x, y, r, g, b);
  strip.show();
}
void draw_pixel_fast(const uint8_t x, const uint8_t y) {
  draw_pixel_raw(x, y, 255, 255, 255);
  strip.show();
}
void draw_pixel_slow_white(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  //for (uint16_t rgb = 0; rgb < 255; rgb += speed_step) {
  uint16_t rgb = 255;
  draw_pixel_raw(x, y, rgb, rgb, rgb);
  strip.show();
  //}
}
void draw_pixel_slow_red(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  for (uint16_t r = 0; r < 255; r += speed_step) {
    draw_pixel_raw(x, y, r, 0, 0);
    strip.show();
  }
}
void draw_pixel_slow_yellow(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  for (uint16_t rg = 0; rg < 255; rg += speed_step) {
    draw_pixel_raw(x, y, rg, rg, 0);
    strip.show();
  }
}
void draw_pixel_slow_green(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  for (uint16_t g = 0; g < 255; g += speed_step) {
    draw_pixel_raw(x, y, 0, g, 0);
    strip.show();
  }
}
void clear_pixel_slow(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  for (uint16_t rgb = 255; rgb <= 10; rgb -= speed_step) {
    draw_pixel_raw(x, y, rgb, rgb, rgb);
    strip.show();
  }
}
void clear_number_fast(const uint8_t x, const uint8_t y) {
  for (uint8_t i = 0; i <= 2; i++) {
    for (uint8_t j = 0; j <= 4; j++) {
      draw_pixel_raw(x + i, y + j, 0, 0, 0);
    }
  }
  strip.show();
}
void fill_number_fast(const uint8_t x, const uint8_t y, byte r, byte g, byte b) {
  for (uint8_t i = 0; i <= 2; i++) {
    for (uint8_t j = 0; j <= 4; j++) {
      draw_pixel_raw(x + i, y + j, r, g, b);
    }
  }
  strip.show();
}
void draw_number_slow(const uint8_t number, const uint8_t x, const uint8_t y) {
  clear_number_fast(x, y);
  switch (number) {
    /////////////////////////////////////////////////////////////////////////////////////////     0
    case 0:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     0
    /////////////////////////////////////////////////////////////////////////////////////////     1
    case 1:
      //step one
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     1
    /////////////////////////////////////////////////////////////////////////////////////////     2
    case 2:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     2
    /////////////////////////////////////////////////////////////////////////////////////////     3
    case 3:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     3
    /////////////////////////////////////////////////////////////////////////////////////////     4
    case 4:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     4
    /////////////////////////////////////////////////////////////////////////////////////////     5
    case 5:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     5
    /////////////////////////////////////////////////////////////////////////////////////////     6
    case 6:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     6
    /////////////////////////////////////////////////////////////////////////////////////////     7
    case 7:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     7
    /////////////////////////////////////////////////////////////////////////////////////////     8
    case 8:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     8
    /////////////////////////////////////////////////////////////////////////////////////////     9
    case 9:
      //step one
      draw_pixel_slow_white(x + 2, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 3, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 2, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step two
      draw_pixel_slow_white(x + 1, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x + 1, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      //step tree
      draw_pixel_slow_white(x, y, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 1, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 2, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      draw_pixel_slow_white(x, y + 4, CONFIG_LED_SPEED_STEPS);
      delay(CONFIG_LED_DELAY_STEPS_mS);
      break;
      /////////////////////////////////////////////////////////////////////////////////////////     9
  }
}
void draw_number_fast(const uint8_t number, const uint8_t x, const uint8_t y) {
  clear_number_fast(x, y);
  switch (number) {
    /////////////////////////////////////////////////////////////////////////////////////////     0
    case 0:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 3);

      draw_pixel_fast(x, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     0
    /////////////////////////////////////////////////////////////////////////////////////////     1
    case 1:
      //step one
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 1);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 3);

      draw_pixel_fast(x + 1, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     1
    /////////////////////////////////////////////////////////////////////////////////////////     2
    case 2:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 3);

      draw_pixel_fast(x, y + 4);

      draw_pixel_fast(x, y);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     2
    /////////////////////////////////////////////////////////////////////////////////////////     3
    case 3:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     3
    /////////////////////////////////////////////////////////////////////////////////////////     4
    case 4:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y + 2);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     4
    /////////////////////////////////////////////////////////////////////////////////////////     5
    case 5:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     5
    /////////////////////////////////////////////////////////////////////////////////////////     6
    case 6:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);
      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 3);

      draw_pixel_fast(x, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     6
    /////////////////////////////////////////////////////////////////////////////////////////     7
    case 7:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      //step tree
      draw_pixel_fast(x, y);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     7
    /////////////////////////////////////////////////////////////////////////////////////////     8
    case 8:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 3);

      draw_pixel_fast(x, y + 4);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     8
    /////////////////////////////////////////////////////////////////////////////////////////     9
    case 9:
      //step one
      draw_pixel_fast(x + 2, y);

      draw_pixel_fast(x + 2, y + 1);

      draw_pixel_fast(x + 2, y + 2);

      draw_pixel_fast(x + 2, y + 3);

      draw_pixel_fast(x + 2, y + 4);

      //step two
      draw_pixel_fast(x + 1, y);

      draw_pixel_fast(x + 1, y + 2);

      draw_pixel_fast(x + 1, y + 4);

      //step tree
      draw_pixel_fast(x, y);

      draw_pixel_fast(x, y + 1);

      draw_pixel_fast(x, y + 2);

      draw_pixel_fast(x, y + 4);

      break;
      /////////////////////////////////////////////////////////////////////////////////////////     9
  }
}
void draw_number_fast_narrow(const uint8_t number, const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  clear_number_fast(x, y);
  switch (number) {
    case 0:
      //step one
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      //step two
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);

      //step tree
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + i + 1, r, g, b);
      }
      break;
    case 1:
      draw_pixel_raw(x, y, r, g, b);
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 1, y + i, r, g, b);
      }
      break;
    case 2:
      draw_pixel_raw(x, y, r, g, b);
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 2, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x, y + 3, r, g, b);
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y + 4, r, g, b);
      }
      break;

    case 3:
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y, r, g, b);
      }
      draw_pixel_raw(x + 2, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x + 2, y + 3, r, g, b);
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y + 4, r, g, b);
      }
      break;
    case 4:
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      break;
    case 5:
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y + 2, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y + 4, r, g, b);
      }
      draw_pixel_raw(x, y + 1, r, g, b);
      draw_pixel_raw(x + 2, y + 3, r, g, b);
      break;
    case 6:
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x, y + i + 1, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + i + 2, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 2, y, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      break;
    case 7:
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      break;
    case 8:
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x, y + 1, r, g, b);
      draw_pixel_raw(x + 2, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x, y + 3, r, g, b);
      draw_pixel_raw(x + 2, y + 3, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      break;
    case 9:
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x, y + 4, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      break;
  }
  strip.show();
}
void draw_point(const uint8_t x, const uint8_t y) {
  draw_pixel_raw(x, y, 255, 255, 90);
  strip.show();
}
void draw_minus(const uint8_t x, const uint8_t y) {
  for (uint8_t i = 0; i <= 2; i++) {
    draw_pixel_now(x + i, y + 2, 255, 255, 255);  //blu
  }
}
void draw_comma(const uint8_t x, const uint8_t y) {
  draw_pixel_now(x, y, 255, 255, 125);  //point
  //draw_pixel_now(x + 7, y + 5, 255, 255, 125);  //point
}
void clean_line(const uint8_t y) {
  clear_number_fast(1, y);
  clear_number_fast(4, y);
  clear_number_fast(7, y);
  clear_number_fast(10, y);
  clear_number_fast(12, y);
}
void fill_line(const uint8_t y, byte r, byte g, byte b) {
  fill_number_fast(1, y, r, g, b);
  fill_number_fast(4, y, r, g, b);
  fill_number_fast(7, y, r, g, b);
  fill_number_fast(10, y, r, g, b);
  fill_number_fast(12, y, r, g, b);
}
void draw_int_string(int16_t number, const uint8_t x, const uint8_t y) {
  uint16_t tens, integer = 0U;
  number = constrain(number, -99, 99);
  //clear_number_fast(x, y);
  if (number < 0) {         //-99
    integer = number * -1;  //-99 -> 99
    draw_minus(x, y + 2);
    draw_number_slow(integer, x + WORK_COLUMN_2, y);
  } else {
    tens = number / 10;
    integer = number % 10;
    draw_number_slow(integer, x + WORK_COLUMN_2, y);
    if (tens == 0) {
      clear_number_fast(x, y);
    } else {
      draw_number_slow(tens, x, y);
    }
  }
}
void draw_float_string(float number, const uint8_t x, const uint8_t y) {
  uint16_t tens, integer, tenths = 0U;
  number = constrain(number, -9.9f, 99.9f);                       //limit number to 4 symbols (xxxx, -12.3)
  uint16_t temp_number = static_cast<int16_t>(number) * 10 * -1;  // 12.3 -> 123

  if (number < 0) {
    draw_minus(x, y);
  } else {
    tens = temp_number / 100;  // 123 -> 1
    if (tens == 0) {
      clear_number_fast(x, y);  //clear "_"x.x
    } else {
      draw_number_slow(tens, x, y);  //draw "1"x.x
    }
  }

  integer = temp_number % 100;                      // 123 -> 23
  integer = integer / 10;                           // 23 -> 2
  draw_number_slow(integer, x + WORK_COLUMN_2, y);  //draw x"2".x
  draw_comma(x + 7, y + 4);                         //draw xx"."x

  tenths = static_cast<uint16_t>(number) % 10;     // 3
  draw_number_slow(tenths, x + WORK_COLUMN_3, y);  //draw xx."3"
}
void draw_humidity_esp(uint8_t number, const uint8_t x, const uint8_t y) {
  uint16_t tens, integer = 0U;
  static uint16_t tens_old, integer_old = 999;
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
  }

  else {
    if (number > 99) number = 99;
    tens = number / 10;
    integer = number % 10;
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + 4, y);
    }
    if (tens != tens_old) {
      tens_old = tens;
      if (tens == 0) {
        clear_number_fast(x, y);
      } else {
        draw_number_slow(tens, x, y);
      }
    }
  }
}
void draw_humidity_aht(uint8_t number, const uint8_t x, const uint8_t y) {
  uint16_t tens, integer = 0U;
  static uint16_t tens_old, integer_old = 999;
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
  }

  else {
    if (number > 99) number = 99;
    tens = number / 10;
    integer = number % 10;
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + 4, y);
    }
    if (tens != tens_old) {
      tens_old = tens;
      if (tens == 0) {
        clear_number_fast(x, y);
      } else {
        draw_number_slow(tens, x, y);
      }
    }
  }
}
void draw_humidity_fast(uint8_t number, const uint8_t x, const uint8_t y) {
  uint16_t tens, integer = 0U;
  if (number > 99) number = 99;
  tens = number / 10;
  integer = number % 10;
  draw_number_fast(integer, x + 4, y);
  draw_number_fast(tens, x, y);
}
void draw_temperature_fast(float number, const uint8_t x, const uint8_t y) {
  int16_t temp_number, tens, integer, tenths = 888;
  temp_number = number * 10;                       // 12.3 -> 123
  temp_number = constrain(temp_number, -99, 999);  //limit number to 4 symbols (xxxx, -12.3)
  if (number < 0) {
    temp_number = temp_number * -1;
    draw_minus(x, y);
  } else {
    tens = temp_number / 100;      // 123 -> 1
    draw_number_fast(tens, x, y);  //draw "1"x.x
  }
  integer = temp_number % 100;                      // 123 -> 23
  integer = integer / 10;                           // 23 -> 2
  draw_number_fast(integer, x + WORK_COLUMN_2, y);  //draw x"2".x
  draw_comma(x + 7, y + 4);                         //draw xx"."x
  tenths = temp_number % 10;                        // 123 -> 3
  draw_number_fast(tenths, x + WORK_COLUMN_3, y);   //draw xx."3"
}
void draw_temperature_esp(float number, const uint8_t x, const uint8_t y) {
  int16_t temp_number, tens, integer, tenths = 888;
  static uint16_t tens_old, integer_old, tenths_old = 999;
  static bool minus = 0;
  static bool minus_old = 0;
  temp_number = number * 10;                       // 12.3 -> 123
  temp_number = constrain(temp_number, -99, 999);  //limit number to 4 symbols (xxxx, -12.3)
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
    tenths_old = ~tenths_old;
  } else {
    if (number < 0) {
      temp_number = temp_number * -1;
      draw_minus(x, y);
      minus = 1;
    } else {
      if (minus != minus_old) {
        minus_old = minus;
        clear_number_fast(x, y);  //clear "-"x.x
        minus = 0;
      }
      tens = temp_number / 100;  // 123 -> 1
      if (tens != tens_old) {
        tens_old = tens;
        if (temp_number > 99) {
          draw_number_slow(tens, x, y);  //draw "1"x.x
        } else {
          clear_number_fast(x, y);  //clear "_"x.x
        }
      }
    }
    integer = temp_number % 100;  // 123 -> 23   110 > 10
    if (integer < 10) {
      integer = 0;
    } else {
      integer = integer / 10;  // 23 -> 2
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + WORK_COLUMN_2, y);  //draw x"2".x
    }
    draw_comma(x + 7, y + 4);   //draw xx"."x
    tenths = temp_number % 10;  // 123 -> 3
    if (tenths != tenths_old) {
      tenths_old = tenths;
      draw_number_slow(tenths, x + WORK_COLUMN_3, y);  //draw xx."3"
    }
  }
}
void draw_temperature_aht(float number, const uint8_t x, const uint8_t y) {
  int16_t temp_number, tens, integer, tenths = 888;
  static uint16_t tens_old, integer_old, tenths_old = 999;
  static bool minus = 0;
  static bool minus_old = 0;
  temp_number = number * 10;                       // 12.3 -> 123
  temp_number = constrain(temp_number, -99, 999);  //limit number to 4 symbols (xxxx, -12.3)
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
    tenths_old = ~tenths_old;
  } else {
    if (number < 0) {
      temp_number = temp_number * -1;
      draw_minus(x, y);
      minus = 1;
    } else {
      if (minus != minus_old) {
        minus_old = minus;
        clear_number_fast(x, y);  //clear "-"x.x
        minus = 0;
      }
      tens = temp_number / 100;  // 123 -> 1
      if (tens != tens_old) {
        tens_old = tens;
        if (temp_number > 99) {
          draw_number_slow(tens, x, y);  //draw "1"x.x
        } else {
          clear_number_fast(x, y);  //clear "_"x.x
        }
      }
    }
    integer = temp_number % 100;  // 123 -> 23   110 > 10
    if (integer < 10) {
      integer = 0;
    } else {
      integer = integer / 10;  // 23 -> 2
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + WORK_COLUMN_2, y);  //draw x"2".x
    }
    draw_comma(x + 7, y + 4);   //draw xx"."x
    tenths = temp_number % 10;  // 123 -> 3
    if (tenths != tenths_old) {
      tenths_old = tenths;
      draw_number_slow(tenths, x + WORK_COLUMN_3, y);  //draw xx."3"
    }
  }
}
void draw_time_esp_fast(uint8_t number, const uint8_t x, const uint8_t y) {
  uint8_t tens, integer = 0U;
  tens = number / 10;
  integer = number % 10;
  draw_number_fast(integer, x + 3, y);
  draw_number_fast(tens, x, y);
}
void draw_hour_esp(uint8_t number, const uint8_t x, const uint8_t y) {
  uint8_t tens, integer = 0U;
  static uint8_t tens_old = 13, integer_old = 25;
  //number = constrain(number, 0, 24);
  tens = number / 10;
  integer = number % 10;
  if (integer != integer_old) {
    integer_old = integer;
    draw_number_slow(integer, x + 3, y);
  }
  if (tens != tens_old) {
    tens_old = tens;
    //if (tens == 0) {
    //  clear_number_fast(x, y);
    //} else {
    draw_number_slow(tens, x, y);
    //}
  }
}
void draw_min_esp(uint8_t number, const uint8_t x, const uint8_t y) {
  uint8_t tens, integer = 0U;
  static uint8_t tens_old, integer_old = 61;
  //number = constrain(number, 0, 60);
  tens = number / 10;     //12 -> 1
  integer = number % 10;  //12 -> 2
  if (tens != tens_old) {
    tens_old = tens;
    draw_number_slow(tens, x, y);
  }
  if (integer != integer_old) {
    integer_old = integer;
    draw_number_slow(integer, x + WORK_COLUMN_2, y);
  }
}
void draw_mday_esp(uint8_t number, const uint8_t x, const uint8_t y) {
  uint8_t tens, integer = 0U;
  static uint8_t tens_old, integer_old = 32;
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
  }

  else {
    number = constrain(number, 1, 31);
    tens = number / 10;
    integer = number % 10;
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + 4, y);
    }
    if (tens != tens_old) {
      tens_old = tens;
      if (tens == 0) {
        clear_number_fast(x, y);
      } else {
        draw_number_slow(tens, x, y);
      }
    }
  }
}
void draw_mon_esp(uint8_t number, const uint8_t x, const uint8_t y) {
  uint8_t tens, integer = 0U;
  static uint8_t tens_old = 13, integer_old = 13;
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
  } else {
    number = constrain(number, 1, 12);  //12
    tens = number / 10;                 // 12/10 = 1
    integer = number % 10;              // 2
    if (tens != tens_old) {
      tens_old = tens;
      //if (tens < 10) clear_number_fast(x, y);
      //else
      draw_number_slow(tens, x, y);
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + 3, y);
    }
  }
}
void mix_color(uint16_t input_value) {  ///yellow rg=255 b=0   //r =255 gb 0
  uint16_t temp_value = constrain(input_value, 400, 1600);
  // 400-2800 red
  buf_color[COLOR_RED] = 255;
  buf_color[COLOR_GREEN] = map(temp_value, 400, 2800, 255, 1);
  buf_color[COLOR_BLUE] = map(temp_value, 400, 2800, 255, 1);
}
void draw_uint_sgp30(uint16_t number, const uint8_t x, const uint8_t y) {
  mix_color(number);
  uint8_t symbol_len = 3;
  uint16_t thousands, hundreds, tens, integer = 0U;
  static uint16_t thousands_old, hundreds_old, tens_old, integer_old = 254U;
  if (number >= 9999) number = 9999;

  thousands = number / 1000;  //1234 -> 1
  hundreds = number % 1000;   //1234 -> 234
  hundreds = hundreds / 100;  //234 -> 2
  tens = number % 100;        //1234 -> 34
  tens = tens / 10;           //34 -> 3
  integer = number % 10;      //1234 -> 4

  if (thousands != thousands_old) {
    thousands_old = thousands;
    if (number < 1000) {
      clear_number_fast(x, y);
    } else {
      draw_number_fast_narrow(thousands, x, y, buf_color[COLOR_RED], buf_color[COLOR_GREEN], buf_color[COLOR_BLUE]);
    }
  }
  if (hundreds != hundreds_old) {
    hundreds_old = hundreds;
    draw_number_fast_narrow(hundreds, x + symbol_len + 1, y, buf_color[COLOR_RED], buf_color[COLOR_GREEN], buf_color[COLOR_BLUE]);
  }
  if (tens != tens_old) {
    tens_old = tens;
    draw_number_fast_narrow(tens, x + symbol_len * 2 + 2, y, buf_color[COLOR_RED], buf_color[COLOR_GREEN], buf_color[COLOR_BLUE]);
  }
  if (integer != integer_old) {
    integer_old = integer;
    draw_number_fast_narrow(integer, x + symbol_len * 3 + 2, y, buf_color[COLOR_RED], buf_color[COLOR_GREEN], buf_color[COLOR_BLUE]);
  }
}
void draw_char(const char text, const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  //clear_number_fast(x, y);
  switch (text) {
    case '%':
      draw_pixel_raw(x, y, r, g, b);
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 1, r, g, b);

      draw_pixel_raw(x + 2, y + 3, r, g, b);
      draw_pixel_raw(x + 3, y + 3, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      draw_pixel_raw(x + 3, y + 4, r, g, b);

      draw_pixel_raw(x, y + 3, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x + 2, y + 2, r, g, b);
      draw_pixel_raw(x + 3, y + 1, r, g, b);


      break;
    case 'a':
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x, y + i + 1, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      break;
    case 'b':
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      break;
    case 'c':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + i + 1, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y + 4, r, g, b);
      }
      break;
    case 'd':
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);

      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + 1 + i, r, g, b);
      }
      break;
    case 'e':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y + 2, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y + 4, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + 1 + i, r, g, b);
      }
      break;
    case 'f':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x, y + 1 + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y + 2, r, g, b);
      }
      break;
    case 'g':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + 1 + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      draw_pixel_raw(x + 2, y + 3, r, g, b);
      break;
    case 'h':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      break;
    case 'i':
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 1, y + i + 2, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      break;
    case 'j':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 1, y + i, r, g, b);
      }
      draw_pixel_raw(x, y + 4, r, g, b);
      break;
    case 'k':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      draw_pixel_raw(x + 2, y, r, g, b);
      draw_pixel_raw(x + 1, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 3, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      break;
    case 'l':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      break;
    case 'm':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 4, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 1, r, g, b);
      draw_pixel_raw(x + 2, y + 2, r, g, b);
      draw_pixel_raw(x + 3, y + 1, r, g, b);
      break;
    case 'n':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      break;
    case 'o':
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + 1 + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      break;
    case 'p':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 3, r, g, b);
      break;
    case 'q':
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x, y + 1 + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      break;
    case 'r':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i + 1, r, g, b);
      }
      draw_pixel_raw(x + 1, y, r, g, b);
      draw_pixel_raw(x + 1, y + 3, r, g, b);
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      break;
    case 's':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1 + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + i, y + 4, r, g, b);
      }
      draw_pixel_raw(x, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x + 2, y + 3, r, g, b);
      break;
    case 't':
      for (uint8_t i = 0; i <= 4; i++) {
        draw_pixel_raw(x + 1, y + i, r, g, b);
      }
      draw_pixel_raw(x, y, r, g, b);
      draw_pixel_raw(x + 2, y, r, g, b);
      break;
    case 'u':
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      break;
    case 'v':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 1, y + i + 2, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 4, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 3, y + i + 2, r, g, b);
      }
      draw_pixel_raw(x + 2, y + 4, r, g, b);
      break;
    case 'w':
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 3; i++) {
        draw_pixel_raw(x + 4, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i + 2, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 4, r, g, b);
      draw_pixel_raw(x + 3, y + 4, r, g, b);
      break;
    case 'x':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x, y + i + 3, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i + 3, r, g, b);
      }
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      break;
    case 'y':
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x, y + i, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + 1, y + i + 2, r, g, b);
      }
      for (uint8_t i = 0; i <= 1; i++) {
        draw_pixel_raw(x + 2, y + i, r, g, b);
      }
      break;
    case 'z':
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y, r, g, b);
      }
      for (uint8_t i = 0; i <= 2; i++) {
        draw_pixel_raw(x + i, y + 3, r, g, b);
      }
      draw_pixel_raw(x + 2, y + 1, r, g, b);
      draw_pixel_raw(x + 1, y + 2, r, g, b);
      draw_pixel_raw(x, y + 3, r, g, b);
      break;
  }
  strip.show();
}
void ini_wifi() {
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s ", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(16, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(16, LOW);
  }
}
void check_wifi() {
  static uint16_t connect_cnt = 0;
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(16, LOW);
    connect_cnt++;
    if (connect_cnt > SYSTEM_CONFIG_WIFI_RECONNECT_S) {
      WiFi.reconnect();
    }
  } else {
    connect_cnt = 0;
    digitalWrite(16, HIGH);
  }
}
void read_wetter_data() {
  static uint16_t cnt = CONFIG_UPDATE_WEATHER_S + 1;
  cnt++;
  if (cnt > CONFIG_UPDATE_WEATHER_S) {  // 15 min
    cnt = 0;
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    jsonBuffer = httpGETRequest(serverPath.c_str());
    //Serial.println(jsonBuffer);
    JSONVar myObject = JSON.parse(jsonBuffer);
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    esp_data.temp = double(myObject["main"]["temp"]) - 273;
    esp_data.humidity = int(myObject["main"]["humidity"]);
  }
}
void read_wetter_data_all() {
  String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
  jsonBuffer = httpGETRequest(serverPath.c_str());
  Serial.println(jsonBuffer);
}
float get_temperature_esp() {
  return static_cast<float>(esp_data.temp);
}
uint8_t get_humidity_esp() {
  return static_cast<uint8_t>(esp_data.humidity);
}
void debug_uart_esp() {
  Serial.print("Temperature ESP: ");
  Serial.print(get_temperature_esp());
  Serial.println(" degrees C");
  Serial.print("Humidity ESP: ");
  Serial.print(get_humidity_esp());
  Serial.println("% rH");
}
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  String payload = "{}";
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    payload = http.getString();
  } else {

    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
uint8_t isSummerTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return false;  // если время не получено — считаем зиму
  }

  int month = timeinfo.tm_mon + 1;  // tm_mon = 0..11
  // апрель–сентябрь = лето
  if (month >= 4 && month <= 9) {
    return 1;
  } else {
    return 0;
  }
}
int get_last_sunday(int year, int month) {
  struct tm t = {};
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = 31;
  mktime(&t);
  // смещение назад до последнего воскресенья
  t.tm_mday -= t.tm_wday;
  mktime(&t);
  return t.tm_mday;
}
int get_dst_offset() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  int year = timeinfo.tm_year + 1900;
  int mon = timeinfo.tm_mon;  // 0-11
  int mday = timeinfo.tm_mday;
  int hour = timeinfo.tm_hour;

  int last_sun_march = get_last_sunday(year, 2);  // март
  int last_sun_oct = get_last_sunday(year, 9);    // октябрь

  if (
    (mon > 2 && mon < 9) ||  // апрель–сентябрь → CEST
    (mon == 2 && (mday > last_sun_march || (mday == last_sun_march && hour >= 2))) || (mon == 9 && (mday < last_sun_oct || (mday == last_sun_oct && hour < 3)))) {
    return 0;  // CEST: UTC+2  //sommer time
  }
  return -1;  // CET: UTC+1   //winter time
}
void set_gmt_offset(long offset) {
  gmtOffset_sec = offset;
}
long get_gmt_offset() {
  return gmtOffset_sec;
}
void ini_time() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void read_time() {
  getLocalTime(&timeinfo);
}
uint8_t get_hour() {
  return timeinfo.tm_hour + isSummerTime();
}
uint8_t get_min() {
  return timeinfo.tm_min;
}
uint8_t get_mday() {
  return timeinfo.tm_mday;
}
uint8_t get_mon() {
  return timeinfo.tm_mon + 1;
}
uint8_t get_sec() {
  return timeinfo.tm_sec;
}
void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
void print_time_colon(const uint8_t x, const uint8_t y) {
  static uint8_t cnt = 0;
  cnt++;
  if (cnt == 1) {
    draw_pixel_now(x, y + 1, CONFIG_COLOR_TIME_COLON);
    draw_pixel_now(x, y + 3, CONFIG_COLOR_TIME_COLON);
  }
  if (cnt == 2) {
    draw_pixel_now(x, y + 1, CONFIG_COLOR_TIME_COLON_BLINK);  //1, 19
    draw_pixel_now(x, y + 3, CONFIG_COLOR_TIME_COLON_BLINK);
    cnt = 0;
  }
}
void read_brightness() {
  static uint16_t cnt = 0;
  cnt++;
  if (cnt > 60) {
    cnt = 0;
    CURRENT_BRIGHTNESS = map(analogRead(A0), 0, 4095, 1, 40);

    strip.setBrightness(CURRENT_BRIGHTNESS);
    strip.show();
  }
}
void set_brightness_1() {
  strip.setBrightness(1);
}
void debug_temt6000() {
  Serial.print("analogRead(A0) = ");
  Serial.print(analogRead(A0));
  Serial.println(" bit");
  Serial.print("brightness in % = ");
  Serial.println(CURRENT_BRIGHTNESS);
}
void ini_buttons() {
  pinMode(BTN_TOUCH_PIN_1, INPUT);
}
void ini_sgp() {
  Wire.begin();

  if (!sgp_30.begin()) {
    Serial.println("Sensor not found :(");
    while (1)
      ;
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp_30.serialnumber[0], HEX);
  Serial.print(sgp_30.serialnumber[1], HEX);
  Serial.println(sgp_30.serialnumber[2], HEX);
  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
}
uint32_t getAbsoluteHumidity(const float temperature, const float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature));  // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                 // [mg/m^3]
  return absoluteHumidityScaled;
}
void read_sgp() {
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  //float temperature = 22.1; // [°C]
  //float humidity = 45.2; // [%RH]
  sgp_30.setHumidity(getAbsoluteHumidity(get_temperature_aht(), static_cast<float>(get_humidity_aht())));

  if (!sgp_30.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
}
uint16_t get_co2_sgp() {
  return sgp_30.eCO2;
}
void debug_uart_sgp() {
  //static auto counter = 0;
  Serial.print("TVOC ");
  Serial.print(sgp_30.TVOC);
  Serial.print(" ppb\t");
  Serial.print("eCO2 ");
  Serial.print(sgp_30.eCO2);
  Serial.println(" ppm");

  if (!sgp_30.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 ");
  Serial.print(sgp_30.rawH2);
  Serial.print(" \t");
  Serial.print("Raw Ethanol ");
  Serial.print(sgp_30.rawEthanol);
  Serial.println("");
  //counter++;
  //if (counter > 30) {
  //counter = 0;
  uint16_t TVOC_base, eCO2_base;
  if (!sgp_30.getIAQBaseline(&eCO2_base, &TVOC_base)) {
    Serial.println("Failed to get baseline readings");
    return;
  }
  Serial.print("****Baseline values: eCO2: 0x");
  Serial.print(eCO2_base, HEX);
  Serial.print(" & TVOC: 0x");
  Serial.println(TVOC_base, HEX);
  //}
}
void ini_aht() {
  Wire1.begin(SDA_PIN_AHT10, SCL_PIN_AHT10);
  if (!aht.begin(&Wire1)) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
}
void read_aht() {
  aht.getEvent(&aht_10_humidity, &aht_10_temperature);  // populate temp and humidity objects with fresh data
}
float get_temperature_aht() {
  return static_cast<float>(aht_10_temperature.temperature);
}
uint8_t get_humidity_aht() {
  return static_cast<uint8_t>(aht_10_humidity.relative_humidity);
}
void debug_uart_aht() {
  Serial.print("Temperature: ");
  Serial.print(aht_10_temperature.temperature);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(aht_10_humidity.relative_humidity);
  Serial.println("% rH");
}
void buzzer_on() {
  ledcWrite(0, 10);
}
void buzzer_off() {
  ledcWrite(0, 0);
}
void ini_ws2812b();
void setup() {
  pinMode(16, OUTPUT);  //LED
  digitalWrite(16, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);  //LED
  digitalWrite(BUZZER_PIN, LOW);

  //Buzzer
  //ledcSetup(0, 5000, 8);  //channel,hz,bit setup PWM OUT for buzzer
  // ledcAttachPin(BUZZER_PIN, 0);   //BUZZER_PIN, PWM_CHANNEL_BUZZER
  //buzzer_on();
  // delay(1000);
  // buzzer_off();
  Serial.begin(9600);  //start UART
  Serial.println("Serial started");
  ini_ws2812b();
  delay(10);
#if AHT_SENSOR
  ini_aht();
#endif
#if SGP_SENSOR
  ini_sgp();
#endif
  ini_wifi();
  ini_time();
  ini_buttons();
  Serial.println("init finish");
  push_all_values();
  set_brightness_1();
}
void update_day() {
#if AHT_SENSOR
  draw_temperature_aht(get_temperature_aht(), 2, WORK_ROW_1);
  draw_point(13, WORK_ROW_1);
  draw_humidity_aht(get_humidity_aht(), 2, WORK_ROW_2);
  draw_char('%', 10, WORK_ROW_2, 255, 255, 90);
#endif
#if SGP_SENSOR
  draw_uint_sgp30(get_co2_sgp(), 1, WORK_ROW_3);
#endif
  draw_hour_esp(get_hour(), 1, WORK_ROW_4);
  draw_min_esp(get_min(), 8, WORK_ROW_4);
  draw_mday_esp(get_mday(), 1, WORK_ROW_5);
  draw_point(8, WORK_ROW_5 + 4);
  draw_mon_esp(get_mon(), 9, WORK_ROW_5);
  draw_temperature_esp(get_temperature_esp(), 2, WORK_ROW_6);
  draw_point(13, WORK_ROW_6);
  draw_humidity_esp(get_humidity_esp(), 2, WORK_ROW_7);
  draw_char('%', 10, WORK_ROW_7, 255, 255, 90);
  //read_brightness();
}
void update_night() {
  clean_line(WORK_ROW_1);
  clean_line(WORK_ROW_2);
#if SGP_SENSOR
  draw_uint_sgp30(get_co2_sgp(), 1, WORK_ROW_3);
#endif
  draw_hour_esp(get_hour(), 1, WORK_ROW_4);
  draw_min_esp(get_min(), 8, WORK_ROW_4);
  clean_line(WORK_ROW_5);
  clean_line(WORK_ROW_6);
  clean_line(WORK_ROW_7);
  // set_brightness_1();
}

void refresh_all_data() {  //1 sec

#if AHT_SENSOR
  read_aht();
#endif
#if SGP_SENSOR
  read_sgp();
#endif
  //check_wifi();
  read_time();
  read_wetter_data();
  print_time_colon(7, WORK_ROW_4);
  static uint16_t cnt = 0;
  cnt++;
  if (cnt > SYSTEM_CONFIG_DISPLAY_UPDATE_S) {
    cnt = 0;
    if (get_hour() > CONFIG_TIME_DAY_START && get_hour() < CONFIG_TIME_DAY_END) {  //day
      update_day();
      day_activated = true;
    } else {  //night
      update_night();
      day_activated = false;
    }
  }
}
void switchCurrentMode(uint8_t mode) {
  static uint8_t mode_old = MODE_NORMAL;
  if (mode_old != mode) {
    setAll(0, 0, 0);
    switch (mode) {
      case MODE_NORMAL:
        delay(100);
        push_all_values();
        break;
      case MODE_FIRE:
        //setAll(0, 0, 0);
        break;
      case MODE_CLOUD:
        //setAll(0, 0, 0);
        break;
      case MODE_RAIN:
        //setAll(0, 0, 0);
        break;
    }
    mode_old = mode;
  }
}
void loop() {
  static unsigned long previousMillis_normal = 0;
  unsigned long currentMillis = millis();
  if (digitalRead(BTN_TOUCH_PIN_1)) {
    CURRENT_MODE++;
    if (CURRENT_MODE > MODE_RAIN) {
      CURRENT_MODE = MODE_NORMAL;
    }
    delay(800);
  }

  /*if(digitalRead(BTN_TOUCH_PIN_2)){
      CURRENT_MODE--;
    if(CURRENT_MODE <= MODE_NORMAL || CURRENT_MODE > =MODE_RAIN){
      CURRENT_MODE = MODE_RAIN;
    }
   delay(800);
  }*/
  switchCurrentMode(CURRENT_MODE);
  //uart_menu_char();

  switch (CURRENT_MODE) {
    case MODE_NORMAL:
      if (currentMillis - previousMillis_normal >= SYSTEM_CONFIG_LOOP_UPDATE_mS) {
        previousMillis_normal = currentMillis;
        refresh_all_data();
      }
      break;
    case MODE_FIRE:
      fire_vertical_advanced();
      break;
    case MODE_CLOUD:
      sand_hourglass();
      break;
    case MODE_RAIN:
      rain_on_window();
      break;
  }
}
void debug_uart() {
  debug_uart_aht();
  debug_uart_sgp();
  debug_temt6000();
  printLocalTime();
  debug_uart_esp();
}
void push_all_values() {
  read_aht();
  read_sgp();
  read_time();
  read_wetter_data();
  draw_point(13, WORK_ROW_1);
  for (float i = -9.9; i < get_temperature_aht(); i += 5) {
    draw_temperature_aht(i, 2, WORK_ROW_1);
  }
  draw_temperature_aht(get_temperature_aht(), 2, WORK_ROW_1);
  draw_char('%', 10, WORK_ROW_2, 255, 255, 90);
  for (uint8_t i = 0; i < get_humidity_aht(); i += 10) {
    draw_humidity_aht(i, 2, WORK_ROW_2);
  }
  draw_humidity_aht(get_humidity_aht(), 2, WORK_ROW_2);
  for (uint16_t i = 0; i < 401; i += 50) {
    draw_uint_sgp30(i, 1, WORK_ROW_3);
  }
  for (uint8_t i = 0; i < get_hour(); i += 5) {
    draw_hour_esp(i, 1, WORK_ROW_4);
  }
  draw_hour_esp(get_hour(), 1, WORK_ROW_4);
  draw_point(8, WORK_ROW_5 + 4);
  for (uint8_t i = 0; i < get_min(); i += 10) {
    draw_min_esp(i, 8, WORK_ROW_4);
  }
  draw_min_esp(get_min(), 8, WORK_ROW_4);

  for (uint8_t i = 0; i < get_mday(); i += 5) {
    draw_mday_esp(i, 1, WORK_ROW_5);
  }
  draw_mday_esp(get_mday(), 1, WORK_ROW_5);

  for (uint8_t i = 0; i < get_mon(); i += 2) {
    draw_mon_esp(i, 9, WORK_ROW_5);
  }
  draw_mon_esp(get_mon(), 9, WORK_ROW_5);

  draw_point(13, WORK_ROW_6);
  for (float i = -9.9; i < get_temperature_esp(); i += 5) {
    draw_temperature_esp(i, 2, WORK_ROW_6);
  }
  draw_temperature_esp(get_temperature_esp(), 2, WORK_ROW_6);

  draw_char('%', 10, WORK_ROW_7, 255, 255, 90);
  for (uint8_t i = 0; i < get_humidity_esp(); i += 10) {
    draw_humidity_esp(i, 2, WORK_ROW_7);
  }
  draw_humidity_esp(get_humidity_esp(), 2, WORK_ROW_7);
}
void uart_menu_char() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    switch (receivedChar) {
      case '?':
        Serial.println("Hello, to set or check values please enter");
        Serial.println("'o' check time offset in sec");
        Serial.println("'s' set sommer time offset(3600s)");
        Serial.println("'w' set winter time offset(0s)");
        Serial.println("'a' refresh CO2 sensor");
        Serial.println("'h' check aht temperature sensor");
        Serial.println("'g' check co2/voc sensor");
        Serial.println("'l' check light sensor");
        Serial.println("'t' check time");
        Serial.println("'e' check weather data");
        break;
      case 'o':
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        break;
      case 's':
        set_gmt_offset(3600);
        Serial.println("now is sommer time !");
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        ini_time();
        get_hour();
        break;
      case 'w':
        set_gmt_offset(0);
        Serial.println("now is winter time !");
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        ini_time();
        get_hour();
        break;
      case 'a':
        ini_sgp();
        Serial.print("CO2 ppn is now: ");
        Serial.println(get_co2_sgp());
        break;
      case 'h':
        debug_uart_aht();
        break;
      case 'g':
        debug_uart_sgp();
        break;
      case 'l':
        debug_temt6000();
        break;
      case 't':
        printLocalTime();
        break;
      case 'e':
        read_wetter_data_all();
        break;
    }
  }
}
void read_uart() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);  // Print the received message in the Serial monitor
      //receivedMessage = "";  // Reset the received message
    } else {
      receivedMessage += receivedChar;  // Append characters to the received message
    }
  }
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}
void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT_LED; i++) {
    setPixel(i, red, green, blue);
  }
  strip.show();
}
void draw_pixel_raw_new(uint8_t x, uint8_t y,
                        uint8_t r, uint8_t g, uint8_t b) {
  // размеры матрицы
  const uint8_t width = 14;   // количество LED по горизонтали (X)
  const uint8_t height = 41;  // количество LED по вертикали (Y)

  // защита от выхода за границы
  if (x >= LED_STRIP_LENGHT || y >= height) return;

  uint16_t index;

  if ((y & 1) == 0) {
    // четная строка: слева направо
    index = y * LED_STRIP_LENGHT + x;
  } else {
    // нечетная строка: справа налево
    index = y * LED_STRIP_LENGHT + (LED_STRIP_LENGHT - 1 - x);
  }

  strip.setPixelColor(index, r, g, b);
}
void fire_vertical_advanced() {
  const uint8_t width = 14;
  const uint8_t height = 41;

  static uint8_t heat[41][14];  // [y][x]
  static float sparks_y[14];    // высота огоньков/искорок для каждого x
  static uint32_t last_ms = 0;
  if (millis() - last_ms < 35) return;  // ~28 FPS
  last_ms = millis();

  // 1. Cooling + мягкое мерцание
  for (uint8_t y = 0; y < height; y++) {
    for (uint8_t x = 0; x < width; x++) {
      uint8_t cooldown = (uint8_t)random(0, 20);
      heat[y][x] = (heat[y][x] > cooldown) ? heat[y][x] - cooldown : 0;

      // мягкое мерцание
      if (random(255) < 15) {
        uint16_t tmp = heat[y][x] + (uint8_t)random(0, 30);
        heat[y][x] = (tmp > 255) ? 255 : (uint8_t)tmp;
      }
    }
  }

  // 2. Распространение тепла вверх с плавной волной
  for (uint8_t x = 0; x < width; x++) {
    for (int y = 0; y < height - 2; y++) {
      int dx = 0;
      if (x > 0 && x < width - 1) dx = random(-1, 2);  // смещение для волны
      uint8_t src_x = constrain(x + dx, 0, width - 1);
      heat[y][x] = (uint8_t)((heat[y + 1][src_x] + heat[y + 2][src_x]) >> 1);
    }
  }

  // 3. Зажигание нижнего ряда
  for (uint8_t x = 0; x < width; x++) {
    if (random(255) < 180) {
      heat[height - 1][x] = (uint8_t)random(180, 255);
    }
  }

  // 4. Случайные искры, которые поднимаются
  for (uint8_t x = 0; x < width; x++) {
    if (random(255) < 20) {
      sparks_y[x] = height - 1;  // старт снизу
    }
    // поднимаем искру вверх
    if (sparks_y[x] > 0) {
      sparks_y[x] -= random(1, 3);  // скорость подъема
      uint8_t sy = (uint8_t)constrain(sparks_y[x], 0, height - 1);
      heat[sy][x] = max(heat[sy][x], (uint8_t)random(150, 255));  // яркая вспышка
    }
  }

  // 5. Отрисовка
  for (uint8_t y = 0; y < height; y++) {
    for (uint8_t x = 0; x < width; x++) {
      uint8_t t = heat[y][x];
      uint8_t r, g, b;

      // градиент цвета от красного к желтому
      if (t > 180) {
        r = 255;
        g = (uint8_t)(200 + min(55, t - 180));
        b = (uint8_t)((t - 180) * 2);
      } else if (t > 100) {
        r = 255;
        g = (uint8_t)((t - 100) * 3);
        b = 0;
      } else {
        r = (uint8_t)(t * 3);
        g = 0;
        b = 0;
      }

      draw_pixel_raw_new(x, y, r, g, b);
    }
  }

  strip.show();
  yield();
}
void sand_hourglass() {
  const uint8_t W = 14;
  const uint8_t H = 41;

  static uint8_t sand[H][W] = {};  // текущее состояние песка
  static uint32_t last_update = 0;

  if (millis() - last_update < 150) return;  // скорость падения
  last_update = millis();

  // ---------- полная маска песочных часов ----------
  static const uint8_t mask[H][W] = {
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },  // ряд 20 — середина
    // Нижняя половина — зеркальное отражение верхней
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
  };

  // ---------- если песок пуст сверху, инициализируем ----------
  bool empty = true;
  for (uint8_t x = 0; x < W; x++)
    if (sand[0][x]) empty = false;
  if (empty) {
    for (uint8_t y = 0; y < H; y++)
      for (uint8_t x = 0; x < W; x++)
        sand[y][x] = mask[y][x];
  }

  // ---------- падение песка ----------
  for (int y = H - 2; y >= 0; y--) {
    for (int x = 0; x < W; x++) {
      if (sand[y][x] && mask[y + 1][x] && sand[y + 1][x] == 0) {
        sand[y + 1][x] = sand[y][x];
        sand[y][x] = 0;
      }
    }
  }

  // ---------- отрисовка ----------
  for (uint8_t y = 0; y < H; y++) {
    for (uint8_t x = 0; x < W; x++) {
      if (sand[y][x]) draw_pixel_raw_new(x, y, 200, 180, 50);  // песок
      else draw_pixel_raw_new(x, y, 0, 0, 0);
    }
  }

  strip.show();
}

void rain_on_window() {
  const uint8_t W = 14;
  const uint8_t H = 41;
  const uint8_t MAX_DROPS = 25;

  static uint32_t last_ms = 0;
  if (millis() - last_ms < 90) return;  // медленное обновление
  last_ms = millis();

  struct Drop {
    float x, y;
    float dx;
    float speed;
    uint8_t length;
    uint8_t brightness;
    bool branch;  // есть разветвление
  };

  static Drop drops[MAX_DROPS] = {};

  // ---------- обновление капель ----------
  for (uint8_t i = 0; i < MAX_DROPS; i++) {
    Drop& d = drops[i];

    if (d.brightness == 0) {
      d.x = random(W);
      d.y = 0;
      d.dx = (random(-10, 11)) / 40.0;  // слегка влево/вправо
      d.speed = random(1, 3) / 3.0;     // разная скорость
      d.length = random(2, 6);
      d.brightness = random(120, 200);
      d.branch = random(0, 5) == 0;  // 20% шанс разветвления
      continue;
    }

    // движение капли
    d.x += d.dx;
    d.y += d.speed;

    // если капля ушла за низ → сброс
    if (d.y >= H) d.brightness = 0;

    // случайное небольшое дрожание/ветер
    if (random(0, 10) == 0) d.dx += ((random(-5, 6)) / 50.0);
  }

  // ---------- очистка ----------
  for (uint8_t y = 0; y < H; y++)
    for (uint8_t x = 0; x < W; x++)
      draw_pixel_raw_new(x, y, 0, 0, 0);

  // ---------- отрисовка ----------
  for (uint8_t i = 0; i < MAX_DROPS; i++) {
    Drop& d = drops[i];
    if (d.brightness == 0) continue;

    int bx = (int)d.x;
    int by_start = (int)d.y;
    int by_end = by_start - d.length + 1;
    if (by_end < 0) by_end = 0;

    // рисуем каплю с градиентом
    for (int by = by_start; by >= by_end; by--) {
      uint8_t b = d.brightness * (by - by_end + 1) / d.length;
      draw_pixel_raw_new(bx, by, 0, 0, b);
    }

    // ---------- разветвление ----------
    if (d.branch && by_start < H - 1) {
      int bx2 = bx + ((random(0, 2) == 0) ? -1 : 1);
      if (bx2 >= 0 && bx2 < W) {
        for (int by = by_start; by >= by_end; by--) {
          uint8_t b = d.brightness / 2;
          draw_pixel_raw_new(bx2, by, 0, 0, b);
        }
      }
    }
  }

  strip.show();
}
