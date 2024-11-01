#include <fonts.h>

uint8_t  UartPixelArray[LED_DISPLAY_LENGTH + 1][LED_DISPLAY_HEIGHT + 1];

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
  pixel(x, y,UART_PIXEL_ON);
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

void printPixelArray() {
  for (int y = 1; y <= LED_DISPLAY_HEIGHT; y++) {
    for (int x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      if ( UartPixelArray[y][x] == UART_PIXEL_ON) {
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
  } else if (c >= '0' && c <= '9') {
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
  }
}

void setChar1x5(char c, int x, int y) {
  if (c == ' ') {
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
  while (*text) {
    if (*text != '*' && *text != '.' && *text != ':' && *text != ' ') {
      setChar3x5(*text, x, startY);
      x += 4;  // Abstand zwischen den Zeichen
    } else {
      setChar1x5(*text, x, startY);
      x += 1;  // Abstand zwischen den Zeichen
    }

    text++;
  }
}

