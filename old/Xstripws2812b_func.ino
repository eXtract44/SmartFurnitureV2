#include <Adafruit_NeoPixel.h>

#define LED_PIN 27
#define strip_len 14                     //14
#define strip_high 41                    //41
#define COUNT_LED strip_len* strip_high  //574

int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel strip(COUNT_LED, LED_PIN, NEO_GRB + NEO_KHZ800);

extern bool day_activated;  // refresh all old data


void ini_ws2812b() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(1);  // Set BRIGHTNESS to about 1/5 (max = 255)
}


#define speed_step_light 250//30  //0-255 Light speed_step animation numbers
#define delay_light 2//30       //mS Light delay animation numbers

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < COUNT_LED; i++) {
    strip.setPixelColor(COUNT_LED, strip.Color(red, green, blue));
  }
  strip.show();
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
  if (y_temp > strip_high) {
    draw_error_pixel();
    return;
    y_temp = strip_high;
  }
  if (x > strip_len) {
    draw_error_pixel();
    return;
    x_temp = strip_len;
  }
  y_temp = y_temp % 2;  // 0 or 1
  if (y_temp == 0) {
    x_temp = strip_len * y - x_temp;  //y == 4
  }
  if (y_temp == 1) {
    x_temp = x_temp + strip_len * (y - 1) - 1;  //y == 5
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
  for (uint16_t rgb = 0; rgb < 255; rgb += speed_step) {
    draw_pixel_raw(x, y, rgb, rgb, rgb);
    strip.show();
  }
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
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     0
    /////////////////////////////////////////////////////////////////////////////////////////     1
    case 1:
      //step one
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     1
    /////////////////////////////////////////////////////////////////////////////////////////     2
    case 2:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     2
    /////////////////////////////////////////////////////////////////////////////////////////     3
    case 3:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     3
    /////////////////////////////////////////////////////////////////////////////////////////     4
    case 4:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     4
    /////////////////////////////////////////////////////////////////////////////////////////     5
    case 5:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     5
    /////////////////////////////////////////////////////////////////////////////////////////     6
    case 6:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     6
    /////////////////////////////////////////////////////////////////////////////////////////     7
    case 7:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     7
    /////////////////////////////////////////////////////////////////////////////////////////     8
    case 8:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     8
    /////////////////////////////////////////////////////////////////////////////////////////     9
    case 9:
      //step one
      draw_pixel_slow_white(x + 2, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 3, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 2, y + 4, speed_step_light);
      delay(delay_light);
      //step two
      draw_pixel_slow_white(x + 1, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x + 1, y + 4, speed_step_light);
      delay(delay_light);
      //step tree
      draw_pixel_slow_white(x, y, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 1, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 2, speed_step_light);
      delay(delay_light);
      draw_pixel_slow_white(x, y + 4, speed_step_light);
      delay(delay_light);
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

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 3 );

      draw_pixel_fast(x, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     0
    /////////////////////////////////////////////////////////////////////////////////////////     1
    case 1:
      //step one
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 1 );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 3 );

      draw_pixel_fast(x + 1, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     1
    /////////////////////////////////////////////////////////////////////////////////////////     2
    case 2:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 3 );

      draw_pixel_fast(x, y + 4 );

      draw_pixel_fast(x, y );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     2
    /////////////////////////////////////////////////////////////////////////////////////////     3
    case 3:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     3
    /////////////////////////////////////////////////////////////////////////////////////////     4
    case 4:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y + 2 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     4
    /////////////////////////////////////////////////////////////////////////////////////////     5
    case 5:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     5
    /////////////////////////////////////////////////////////////////////////////////////////     6
    case 6:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );
      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 3 );

      draw_pixel_fast(x, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     6
    /////////////////////////////////////////////////////////////////////////////////////////     7
    case 7:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      //step tree
      draw_pixel_fast(x, y );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     7
    /////////////////////////////////////////////////////////////////////////////////////////     8
    case 8:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 3 );

      draw_pixel_fast(x, y + 4 );

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     8
    /////////////////////////////////////////////////////////////////////////////////////////     9
    case 9:
      //step one
      draw_pixel_fast(x + 2, y );

      draw_pixel_fast(x + 2, y + 1 );

      draw_pixel_fast(x + 2, y + 2 );

      draw_pixel_fast(x + 2, y + 3 );

      draw_pixel_fast(x + 2, y + 4 );

      //step two
      draw_pixel_fast(x + 1, y );

      draw_pixel_fast(x + 1, y + 2 );

      draw_pixel_fast(x + 1, y + 4 );

      //step tree
      draw_pixel_fast(x, y );

      draw_pixel_fast(x, y + 1 );

      draw_pixel_fast(x, y + 2 );

      draw_pixel_fast(x, y + 4 );

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
#define NUMBER_2_POSITION_X 4  // x "x".x
#define NUMBER_3_POSITION_X 8  // x  x ."x"
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
    draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);
  } else {
    tens = number / 10;
    integer = number % 10;
    draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);
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

  integer = temp_number % 100;                            // 123 -> 23
  integer = integer / 10;                                 // 23 -> 2
  draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);  //draw x"2".x
  draw_comma(x + 7, y + 4);                               //draw xx"."x

  tenths = static_cast<uint16_t>(number) % 10;           // 3
  draw_number_slow(tenths, x + NUMBER_3_POSITION_X, y);  //draw xx."3"
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
  temp_number = number * 10;                 // 12.3 -> 123
  temp_number = constrain(temp_number, -99, 999);  //limit number to 4 symbols (xxxx, -12.3)
  if (number < 0) {
    temp_number = temp_number * -1;
    draw_minus(x, y);
  } else {
    tens = temp_number / 100;  // 123 -> 1
    draw_number_fast(tens, x, y);  //draw "1"x.x
  }
  integer = temp_number % 100;  // 123 -> 23
  integer = integer / 10;       // 23 -> 2
  draw_number_fast(integer, x + NUMBER_2_POSITION_X, y);  //draw x"2".x
  draw_comma(x + 7, y + 4);   //draw xx"."x
  tenths = temp_number % 10;  // 123 -> 3
  draw_number_fast(tenths, x + NUMBER_3_POSITION_X, y);  //draw xx."3"
}
void draw_temperature_esp(float number, const uint8_t x, const uint8_t y) {
  int16_t temp_number, tens, integer, tenths = 888;
  static uint16_t tens_old, integer_old, tenths_old = 999;
  static bool minus = 0;
  static bool minus_old = 0;
  temp_number = number * 10;                 // 12.3 -> 123
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
      integer = integer / 10;       // 23 -> 2
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);  //draw x"2".x
    }
    draw_comma(x + 7, y + 4);   //draw xx"."x
    tenths = temp_number % 10;  // 123 -> 3
    if (tenths != tenths_old) {
      tenths_old = tenths;
      draw_number_slow(tenths, x + NUMBER_3_POSITION_X, y);  //draw xx."3"
    }
  }
}
void draw_temperature_aht(float number, const uint8_t x, const uint8_t y) {
  int16_t temp_number, tens, integer, tenths = 888;
  static uint16_t tens_old, integer_old, tenths_old = 999;
  static bool minus = 0;
  static bool minus_old = 0;
  temp_number = number * 10;                 // 12.3 -> 123
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
      integer = integer / 10;       // 23 -> 2
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);  //draw x"2".x
    }
    draw_comma(x + 7, y + 4);   //draw xx"."x
    tenths = temp_number % 10;  // 123 -> 3
    if (tenths != tenths_old) {
      tenths_old = tenths;
      draw_number_slow(tenths, x + NUMBER_3_POSITION_X, y);  //draw xx."3"
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
  static uint8_t tens_old, integer_old = 25;
  //number = constrain(number, 0, 24);
  tens = number / 10;
  integer = number % 10;
  if (integer != integer_old) {
    integer_old = integer;
    draw_number_slow(integer, x + 3, y);
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
    draw_number_slow(integer, x + NUMBER_2_POSITION_X, y);
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
  static uint8_t tens_old, integer_old = 13;
  if (!day_activated) {
    tens_old = ~tens_old;
    integer_old = ~integer_old;
  } else {
    number = constrain(number, 1, 12);
    tens = number / 10;
    integer = number % 10;
    if (tens != tens_old) {
      tens_old = tens;
      if (tens < 10) clear_number_fast(x, y);
      else draw_number_slow(tens, x, y);
    }
    if (integer != integer_old) {
      integer_old = integer;
      draw_number_slow(integer, x + 3, y);
    }
  }
}
#define RED 0
#define GREEN 1
#define BLUE 2
uint8_t buf_color[3] = {255, 255, 255};
void mix_color(uint16_t input_value) {  ///yellow rg=255 b=0   //r =255 gb 0
  uint16_t temp_value = constrain(input_value, 400, 1600);
  // 400-2800 red
  buf_color[RED] = 255;
  buf_color[GREEN] = map(temp_value, 400, 2800, 255, 1);
  buf_color[BLUE] = map(temp_value, 400, 2800, 255, 1);
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
      draw_number_fast_narrow(thousands, x, y, buf_color[RED], buf_color[GREEN], buf_color[BLUE]);
    }
  }
  if (hundreds != hundreds_old) {
    hundreds_old = hundreds;
    draw_number_fast_narrow(hundreds, x + symbol_len + 1, y,  buf_color[RED], buf_color[GREEN], buf_color[BLUE]);
  }
  if (tens != tens_old) {
    tens_old = tens;
    draw_number_fast_narrow(tens, x + symbol_len * 2 + 2, y, buf_color[RED], buf_color[GREEN], buf_color[BLUE]);
  }
  if (integer != integer_old) {
    integer_old = integer;
    draw_number_fast_narrow(integer, x + symbol_len * 3 + 2, y, buf_color[RED], buf_color[GREEN], buf_color[BLUE]);
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
