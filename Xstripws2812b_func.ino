#include <Adafruit_NeoPixel.h>

#define LED_PIN 27
#define strip_len 14                     //14
#define strip_high 41                    //41
#define COUNT_LED strip_len* strip_high  //574
#define speed_step_light 250//30  //0-255 Light speed_step animation numbers
#define delay_light 2//30       //mS Light delay animation numbers

int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel strip(COUNT_LED, LED_PIN, NEO_GRB + NEO_KHZ800);

extern bool day_activated;  // refresh all old data


void ini_ws2812b() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(1);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < COUNT_LED; i++) {
    strip.setPixelColor(COUNT_LED, strip.Color(red, green, blue));
  }
  strip.show();
}
void log_invalid_pixel_coords(const uint8_t x, const uint8_t y) {
  Serial.print("Invalid pixel coordinates: x=");
  Serial.print(x);
  Serial.print(", y=");
  Serial.println(y);
}
void draw_pixel_raw(const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  if (y >= strip_high || x >= strip_len) {
    log_invalid_pixel_coords(x,y);
    return;
  }

  uint16_t index;
  bool is_odd_row = y % 2;

  if (is_odd_row) {
    index = y * strip_len + x;
  } else {
    index = y * strip_len + (strip_len - 1 - x);
  }

  strip.setPixelColor(index, strip.Color(r, g, b));
}
void draw_pixel_immediate(const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  draw_pixel_raw(x, y, r, g, b);
  strip.show();
}
void draw_pixel_white(const uint8_t x, const uint8_t y) {
  draw_pixel_raw(x, y, 255, 255, 255);
  strip.show();
}

void fade_in_white_pixel(const uint8_t x, const uint8_t y, const uint8_t step) {
  if (step == 0) return; // защита от бесконечного цикла

  for (uint16_t rgb = 0; rgb <= 255; rgb += step) {
    draw_pixel_raw(x, y, rgb, rgb, rgb);
    strip.show();
  }
}

void fade_in_red_pixel(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  if (step == 0) return; // защита от бесконечного цикла
  for (uint16_t r = 0; r < 255; r += speed_step) {
    draw_pixel_raw(x, y, r, 0, 0);
    strip.show();
  }
}
void fade_in_yellow_pixel(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  if (step == 0) return; // защита от бесконечного цикла
  for (uint16_t rg = 0; rg < 255; rg += speed_step) {
    draw_pixel_raw(x, y, rg, rg, 0);
    strip.show();
  }
}
void fade_in_green_pixel(const uint8_t x, const uint8_t y, const uint8_t speed_step) {
  if (step == 0) return; // защита от бесконечного цикла
  for (uint16_t g = 0; g < 255; g += speed_step) {
    draw_pixel_raw(x, y, 0, g, 0);
    strip.show();
  }
}

void fade_out_pixel(const uint8_t x, const uint8_t y, const uint8_t step) {
  if (step == 0) return;

  for (int rgb = 255; rgb >= 0; rgb -= step) {
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

struct PixelPos {
  uint8_t x_offset;
  uint8_t y_offset;
};

void fade_in_sequence_fast(const uint8_t base_x, const uint8_t base_y, const PixelPos* positions, size_t count, uint8_t speed_step, uint16_t delay_ms) {
  for (size_t i = 0; i < count; i++) {
    fade_in_white_pixel(base_x + positions[i].x_offset, base_y + positions[i].y_offset, speed_step);
  }
}

void fade_in_sequence_slow(const uint8_t base_x, const uint8_t base_y, const PixelPos* positions, size_t count, uint8_t speed_step, uint16_t delay_ms) {
  for (size_t i = 0; i < count; i++) {
    fade_in_white_pixel(base_x + positions[i].x_offset, base_y + positions[i].y_offset, speed_step);
    delay(delay_ms);
  }
}

void draw_number_slow(const uint8_t number, const uint8_t x, const uint8_t y) {
  clear_number_fast(x, y);

  // Координаты для каждой цифры (пример для 0–9)
  static const PixelPos digits[][20] = {
    // 0
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 1
    { {1,0},{1,1},{1,2},{1,3},{1,4} },
    // 2
    { {2,0},{2,1},{2,2},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,2},{0,3},{0,4} },
    // 3
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,2},{0,4} },
    // 4
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,2},
      {0,0},{0,1},{0,2} },
    // 5
    { {2,0},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,4} },
    // 6
    { {2,0},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 7
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},
      {0,0} },
    // 8
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 9
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,4} }
  };

  // Количество точек для каждой цифры (по массиву выше)
  static const size_t digits_len[] = {12, 5, 11, 9, 6, 11, 13, 7, 13, 12};

  if (number > 9) return; // Защита от неверных входных данных

  fade_in_sequence(x, y, digits[number], digits_len[number], speed_step_light, delay_light);
}


void draw_number_fast(const uint8_t number, const uint8_t x, const uint8_t y) {
  clear_number_fast(x, y);

  static const PixelPos digits[][20] = {
    // 0
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 1
    { {1,0},{1,1},{1,2},{1,3},{1,4} },
    // 2
    { {2,0},{2,1},{2,2},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,2},{0,3},{0,4} },
    // 3
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,2},{0,4} },
    // 4
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,2},
      {0,0},{0,1},{0,2} },
    // 5
    { {2,0},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,4} },
    // 6
    { {2,0},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 7
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},
      {0,0} },
    // 8
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,3},{0,4} },
    // 9
    { {2,0},{2,1},{2,2},{2,3},{2,4},
      {1,0},{1,2},{1,4},
      {0,0},{0,1},{0,2},{0,4} }
  };

  static const size_t digits_len[] = {12, 5, 11, 9, 6, 11, 13, 7, 13, 12};

  if (number > 9) return;

  draw_white_sequence_fast(x, y, digits[number], digits_len[number]);
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
    draw_pixel_immediate(x + i, y + 2, 255, 255, 255);  //blu
  }
}
void draw_comma(const uint8_t x, const uint8_t y) {
  draw_pixel_immediate(x, y, 255, 255, 125);  //point
  //draw_pixel_immediate(x + 7, y + 5, 255, 255, 125);  //point
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
