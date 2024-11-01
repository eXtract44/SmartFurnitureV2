#include <fonts.h>

uint8_t uartPixelArray[LED_DISPLAY_LENGTH + 1][LED_DISPLAY_HEIGHT + 1];

#define UART_PIXEL_ON 1
#define UART_PIXEL_OFF 0

void uartPixel(unsigned int x, unsigned int y, char set) {
  if (x > 0 && x <= LED_DISPLAY_LENGTH && y > 0 && y <= LED_DISPLAY_HEIGHT) {
    uartPixelArray[y][x] = set;  // Setze den Pixel-Wert (1 oder 0)
  } else {
    printf("Fehler: Pixelkoordinaten (%d, %d) liegen außerhalb der Grenzen!\n", x, y);
  }
}

void setUartPixel(unsigned int x, unsigned int y) {
  uartPixel(x, y, UART_PIXEL_ON);
}

void clearUartPixel(unsigned int x, unsigned int y) {
  uartPixel(x, y, UART_PIXEL_OFF);
}

void cleanUartArray() {
  for (int y = 1; y <= LED_DISPLAY_HEIGHT; y++) {
    for (int x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      uartPixelArray[y][x] = UART_PIXEL_OFF;
    }
  }
}

void printUartArray() {
  for (int y = 1; y <= LED_DISPLAY_HEIGHT; y++) {
    for (int x = 1; x <= LED_DISPLAY_LENGTH; x++) {
      if (uartPixelArray[y][x] == UART_PIXEL_ON) {
        printf("*");
      } else {
        printf(".");
      }
    }
    printf("\n");  // Neue Zeile nach jeder Zeile im Array
  }
  printf("*************************************************************\n");
}

void printNumber(const uint8_t number, const uint8_t x, const uint8_t y, char set) {
  switch (number) {
    /////////////////////////////////////////////////////////////////////////////////////////     0
    case 0:
      //step one
      uartPixel(x + 2, y, set);
      uartPixel(x + 2, y + 1, set);
      uartPixel(x + 2, y + 2, set);
      uartPixel(x + 2, y + 3, set);
      uartPixel(x + 2, y + 4, set);
      //step two
      uartPixel(x + 1, y, set);
      uartPixel(x + 1, y + 4, set);
      //step tree
      uartPixel(x, y, set);
      uartPixel(x, y + 1, set);
      uartPixel(x, y + 2, set);
      uartPixel(x, y + 3, set);
      uartPixel(x, y + 4, set);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     0
    /////////////////////////////////////////////////////////////////////////////////////////     1
    case 1:
      //step one
      uartPixel(x + 1, y, set);
      uartPixel(x + 1, y + 1, set);
      uartPixel(x + 1, y + 2, set);
      uartPixel(x + 1, y + 3, set);
      uartPixel(x + 1, y + 4, set);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     1
    /////////////////////////////////////////////////////////////////////////////////////////     2
    case 2:
      //step one
      uartPixel(x + 2, y, set);
      uartPixel(x + 2, y + 1, set);
      uartPixel(x + 2, y + 2, set);
      uartPixel(x + 2, y + 4, set);
      //step two
      uartPixel(x + 1, y, set);
      uartPixel(x + 1, y + 2, set);
      uartPixel(x + 1, y + 4, set);
      //step tree
      uartPixel(x, y + 2, set);
      uartPixel(x, y + 3, set);
      uartPixel(x, y + 4, set);
      uartPixel(x, y, set);
      break;
    /////////////////////////////////////////////////////////////////////////////////////////     2
    /////////////////////////////////////////////////////////////////////////////////////////     3
    case 3:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 1, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y, set);

      uartPixel(x + 1, y + 2, set);

      uartPixel(x + 1, y + 4, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 2, set);

      uartPixel(x, y + 4, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     3
    /////////////////////////////////////////////////////////////////////////////////////////     4
    case 4:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 1, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y + 2, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 1, set);

      uartPixel(x, y + 2, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     4
    /////////////////////////////////////////////////////////////////////////////////////////     5
    case 5:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y, set);

      uartPixel(x + 1, y + 2, set);

      uartPixel(x + 1, y + 4, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 1, set);

      uartPixel(x, y + 2, set);

      uartPixel(x, y + 4, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     5
    /////////////////////////////////////////////////////////////////////////////////////////     6
    case 6:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);
      //step two
      uartPixel(x + 1, y, set);

      uartPixel(x + 1, y + 2, set);

      uartPixel(x + 1, y + 4, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 1, set);

      uartPixel(x, y + 2, set);

      uartPixel(x, y + 3, set);

      uartPixel(x, y + 4, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     6
    /////////////////////////////////////////////////////////////////////////////////////////     7
    case 7:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 1, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y, set);

      //step tree
      uartPixel(x, y, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     7
    /////////////////////////////////////////////////////////////////////////////////////////     8
    case 8:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 1, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y, set);

      uartPixel(x + 1, y + 2, set);

      uartPixel(x + 1, y + 4, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 1, set);

      uartPixel(x, y + 2, set);

      uartPixel(x, y + 3, set);

      uartPixel(x, y + 4, set);

      break;
    /////////////////////////////////////////////////////////////////////////////////////////     8
    /////////////////////////////////////////////////////////////////////////////////////////     9
    case 9:
      //step one
      uartPixel(x + 2, y, set);

      uartPixel(x + 2, y + 1, set);

      uartPixel(x + 2, y + 2, set);

      uartPixel(x + 2, y + 3, set);

      uartPixel(x + 2, y + 4, set);

      //step two
      uartPixel(x + 1, y, set);

      uartPixel(x + 1, y + 2, set);

      uartPixel(x + 1, y + 4, set);

      //step tree
      uartPixel(x, y, set);

      uartPixel(x, y + 1, set);

      uartPixel(x, y + 2, set);

      uartPixel(x, y + 4, set);

      break;
      /////////////////////////////////////////////////////////////////////////////////////////     9
  }
}

void cleanNumber(const uint8_t number, const uint8_t x, const uint8_t y) {
  printNumber(number, x, y, UART_PIXEL_OFF);
}

void setNumber(const uint8_t number, const uint8_t x, const uint8_t y) {
  printNumber(number, x, y, UART_PIXEL_ON);
}
void drawPixel(int x, int y, uint32_t color) {
  setUartPixel(x, y);
}
void drawChar3x5(char c, int x, int y, uint32_t color) {
  if (c >= 'A' && c <= 'Z') {
    uint8_t *bitmap = font3x5[c - 'A'];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          drawPixel(x + col, y + row, color);
        }
      }
    }
  } else if (c >= '0' && c <= '9') {
    uint8_t *bitmap = font3x5[c - '0' + 26];
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        if (bitmap[row] & (1 << (2 - col))) {
          drawPixel(x + col, y + row, color);
        }
      }
    }
  }
  else if (c == '%') {
    *  *
      * 
          drawPixel(x , y , color);   
          drawPixel(x+3 , y , color); 

          drawPixel(x +2, y+1 , color);

          drawPixel(x +1, y+2 , color);
          drawPixel(x+3 , y+4 , color); 

  }
}

void drawChar1x5(char c, int x, int y, uint32_t color) {
  if (c == ' ') {
  } else if (c == ':') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 3, color);
  } else if (c == '.') {
    drawPixel(x, y+4, color);
  } else if (c == '*') {
    drawPixel(x, y, color);
  }
}

void displayText3x5(const char *text, int startX, int startY, uint32_t color) {
  int x = startX;
  while (*text) {
    if (*text != '*' && *text != '.' && *text != ':' && *text != ' ') {
      drawChar3x5(*text, x, startY, color);
      x += 4;  // Abstand zwischen den Zeichen
    } else {
      drawChar1x5(*text, x, startY, color);
      x += 1;  // Abstand zwischen den Zeichen
    }

    text++;
  }
}