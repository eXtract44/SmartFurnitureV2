#include <fonts.h>

uint8_t UartPixelArray[LED_DISPLAY_LENGTH + 1][LED_DISPLAY_HEIGHT + 1];

#define UART_PIXEL_ON 1
#define UART_PIXEL_OFF 0

void pixel(unsigned int x, unsigned int y, char set) {
  if (x > 0 && x <= LED_DISPLAY_LENGTH && y > 0 && y <= LED_DISPLAY_HEIGHT) {
    UartPixelArray[y][x] = set;  // Setze den Pixel-Wert (1 oder 0)
  } else {
    printf("Fehler: Pixelkoordinaten (%d, %d) liegen außerhalb der Grenzen!\n", x, y);
  }
}

void setPixel(unsigned int x, unsigned int y) {
  pixel(x, y, UART_PIXEL_ON);
}

void clearPixel(unsigned int x, unsigned int y) {
  pixel(x, y, UART_PIXEL_OFF);
}

void setAllpixelOFF() {
  for (int y = 1; y <= LED_DISPLAY_HEIGHT; y++) {
    for (int x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      UartPixelArray[y][x] = UART_PIXEL_OFF;
    }
  }
}
void setLinePixelOFF(uint8_t line) {
  //TODO
  if (line != 1) {
    for (uint8_t x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      pixel(x, line * 6 + 1, 0);
      pixel(x, line * 6 + 2, 0);
      pixel(x, line * 6 + 3, 0);
      pixel(x, line * 6 + 4, 0);
      pixel(x, line * 6 + 5, 0);
    }
  } else {
    for (uint8_t x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      pixel(x, line, 0);
      pixel(x, line + 1, 0);
      pixel(x, line + 2, 0);
      pixel(x, line + 3, 0);
      pixel(x, line + 4, 0);
    }
  }
}
void printPixelArray() {
  for (int y = 1; y <= LED_DISPLAY_HEIGHT; y++) {
    for (int x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      if (UartPixelArray[y][x] == UART_PIXEL_ON) {
        printf("*");
      } else {
        printf(".");
      }
    }
    printf("\n");  // Neue Zeile nach jeder Zeile im Array
  }
  printf("*************************************************************\n");
}

void setChar3x5(char c, int x, int y) {
  if (c >= 'A' && c <= 'Z') {
    uint8_t *bitmap = font3x5[c - 'A'];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          setPixel(x + col, y + row);
        }
      }
    }
  } else if (c >= '0' && c <= '9' && c != '1') {
    uint8_t *bitmap = font3x5[c - '0' + 26];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          setPixel(x + col, y + row);
        }
      }
    }
  } else if (c == '%') {
    setPixel(x, y);
    setPixel(x + 3, y);
    setPixel(x + 3, y + 1);
    setPixel(x + 2, y + 2);
    setPixel(x, y + 3);
    setPixel(x, y + 4);
    setPixel(x + 3, y + 4);
  } else if (c == '1') {
    setPixel(x, y);
    setPixel(x, y + 1);
    setPixel(x, y + 2);
    setPixel(x, y + 3);
    setPixel(x, y + 4);
  }

  else if (c == ' ') {

  } else if (c == ':') {
    setPixel(x, y + 1);
    setPixel(x, y + 3);
  } else if (c == '.') {
    setPixel(x, y + 4);
  } else if (c == '*') {
    setPixel(x, y);
  }
}

void setText3x5(const char *text, int startX, int startY) {
  int x = startX;

  int line4 = 0;
  int line5 = 0;
  while (*text) {
    char prev_char = *text;
    if (*text == '*' || *text == ' ') {
      setChar3x5(*text, x, startY);
      x += 1;  // Abstand zwischen den Zeichen
    } else if (*text == '1') {
      setChar3x5(*text, x, startY);
      x += 2;  // Abstand zwischen den Zeichen
    } else if (*text == ':') {
      setChar3x5(*text, x - 1, startY);
    } else if (*text == '.') {
      setChar3x5(*text, x - 1, startY);
    } else if (startY == LED_DISPLAY_LINE_4){ 
        line4++;
        setChar3x5(*text, x, startY);
        if (line4 == 1)
          x += 4;  // Abstand zwischen den Zeichen
        else
          x += 4;  // Abstand zwischen den Zeichen
    } /*else if (startY == LED_DISPLAY_LINE_5){ 
        line5++;
        setChar3x5(*text, x, startY);
        if (line5 == 2)
          x += 3;  // Abstand zwischen den Zeichen
        else
          x += 4;  // Abstand zwischen den Zeichen
    }*/else {
      setChar3x5(*text, x, startY);
      x += 4;  // Abstand zwischen den Zeichen
    }

    text++;
  }
  line4 = 0;
  line5 = 0;
}
