#if LED_DISPLAY

#include <config_.h>
#include <fonts.h>




void ledDisplayInit() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(1);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

void drawAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_DISPLAY_COUNT_LED; i++) {
    strip.setPixelColor(LED_DISPLAY_COUNT_LED, strip.Color(red, green, blue));
  }
  strip.show();
}

void drawLedAllPixelOFF() {
  setAll(0, 0, 0);
}

void drawLedLinePixelOFF(uint8_t line) {
  //TODO
  if (line != 1) {
    for (uint8_t x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      setLedPixel(x, line * 6 + 1, 0);
      setLedPixel(x, line * 6 + 2, 0);
      setLedPixel(x, line * 6 + 3, 0);
      setLedPixel(x, line * 6 + 4, 0);
      setLedPixel(x, line * 6 + 5, 0);
    }
  } else {
    for (uint8_t x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      setLedPixel(x, line, 0);
      setLedPixel(x, line + 1, 0);
      setLedPixel(x, line + 2, 0);
      setLedPixel(x, line + 3, 0);
      setLedPixel(x, line + 4, 0);
    }
  }
}

void setLedPixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < LED_DISPLAY_LENGTH && y >= 0 && y < LED_DISPLAY_HEIGHT) {
    int pixelIndex = y * LED_DISPLAY_LENGTH + x;
    strip.setPixelColor(pixelIndex, color);
  }
}

void setLedChar3x5(char c, int x, int y, uin32_t color) {
  if (c >= 'A' && c <= 'Z') {
    uint8_t *bitmap = font3x5[c - 'A'];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          setLedPixel(x + col, y + row, uin32_t color);
        }
      }
    }
  } else if (c >= '0' && c <= '9') {
    uint8_t *bitmap = font3x5[c - '0' + 26];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          setLedPixel(x + col, y + row, uin32_t color);
        }
      }
    }
  } else if (c == '%') {
    setLedPixel(x, y, uin32_t color);
    setLedPixel(x + 3, y, uin32_t color);
    setLedPixel(x + 3, y + 1, uin32_t color);
    setLedPixel(x + 2, y + 2, uin32_t color);
    setLedPixel(x, y + 3, uin32_t color);
    setLedPixel(x, y + 4, uin32_t color);
    setLedPixel(x + 3, y + 4, uin32_t color);
  }
}

void setLedChar1x5(char c, int x, int y) {
  if (c == ' ') {
  } else if (c == ':') {
    setLedPixel(x, y + 1, uin32_t color);
    setLedPixel(x, y + 3, uin32_t color);
  } else if (c == '.') {
    setLedPixel(x, y + 4, uin32_t color);
  } else if (c == '*') {
    setLedPixel(x, y, uin32_t color);
  }
}

void setLedText3x5(const char *text, int startX, int startY, uin32_t color) {
  int x = startX;
  while (*text) {
    if (*text != '*' && *text != '.' && *text != ':' && *text != ' ') {
      setLedChar3x5(*text, x, startY, uin32_t color);
      x += 4;  // Abstand zwischen den Zeichen
    } else {
      setLedChar1x5(*text, x, startY, uin32_t color);
      x += 1;  // Abstand zwischen den Zeichen
    }

    text++;
  }
}


















































#endif
