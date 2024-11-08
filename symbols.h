
#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include "config_.h"

const uint8_t homeInside10x11[] = {
  0x00, 0x00, 0x00, 0x00, 0x6d, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x92, 0x92, 0x49, 0xb6, 0x24, 0x00, 0x00, 0x00, 0x24, 0x92, 0x00, 0x00, 0x92, 0xff, 0x24, 0x00, 0x00, 0x24, 0x92, 0x24, 0x00, 0x00, 0x24, 0xdb, 0x24, 0x00, 0x00, 0x92, 0x49, 0x00, 0x00, 0x00, 0x00, 0x49, 0x92, 0x00, 0x92, 0xdb, 0x00, 0x24, 0x6d, 0x6d, 0x24, 0x00, 0xdb, 0x92, 0xb6, 0xdb, 0x00, 0x49, 0xdb, 0xdb, 0x49, 0x00, 0xdb, 0xb6, 0x00, 0xdb, 0x00, 0x49, 0x92, 0x92, 0x49, 0x00, 0xdb, 0x00, 0x00, 0xdb, 0x00, 0x49, 0x92, 0x92, 0x49, 0x00, 0xdb, 0x00, 0x00, 0xdb, 0x00, 0x6d, 0x92, 0x92, 0x6d, 0x00, 0xdb, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00
};

const uint8_t temperatureInside5x9[] = {
  0x00,0x00,0xfc,0x00,0x00,0x00,0xfc,0xe0,0xfc,0x00,0x00,0xfc,0xe0,0xfc,0x00
,0x00,0xfc,0xe0,0xfc,0x00,0x00,0xfc,0xe0,0xfc,0x00,0x00,0xfc,0xe0,0xfc,0x00
,0xfc,0x00,0xe0,0x00,0xfc,0xfc,0x00,0x00,0x00,0xfc,0x00,0xfc,0xfc,0xfc,0x00
};
void draw_pixel_raw(const unsigned int x, const unsigned int y, const uint8_t r, const uint8_t g, const uint8_t b) {
  unsigned int y_temp = y;
  unsigned int x_temp = x;
  if (y_temp > LED_DISPLAY_HEIGHT) {
    printf("Fehler: Pixelkoordinaten (x %d, y %d) liegen außerhalb der Grenzen!\n", x, y); 
    return;
  }
  if (x > LED_DISPLAY_LENGTH) {
    printf("Fehler: Pixelkoordinaten (x %d, y %d) liegen außerhalb der Grenzen!\n", x, y); 
    return;
  }
  y_temp = y_temp % 2;  // 0 or 1
  if (y_temp == 0) {
    x_temp = LED_DISPLAY_LENGTH * y - x_temp;  //y == 4
  }
  if (y_temp == 1) {
    x_temp = x_temp + LED_DISPLAY_LENGTH * (y - 1) - 1;  //y == 5
  }
  strip.setPixelColor(x_temp, strip.Color(r, g, b));
}
void drawImage(const uint8_t *image, const unsigned int matrixWidth, const unsigned int matrixHeight, const unsigned int startX, const unsigned int startY) {
  int index = 0;

  // Loop through all pixels in the matrix
  for (int y = 0; y < matrixHeight; y++) {
    for (int x = 0; x < matrixWidth; x++) {
      // Calculate the corresponding image coordinates
      int imageX = x + startX;
      int imageY = y + startY;

      // Check if the image coordinates are within the image boundaries
      if (imageX >= 0 && imageX < matrixWidth && imageY >= 0 && imageY < matrixHeight) {
        // Calculate the position in the image array
        index = (imageY * matrixWidth + imageX) * 3;

        // Get the RGB values from the image array
        uint8_t r = image[index];
        uint8_t g = image[index + 1];
        uint8_t b = image[index + 2];

        // Calculate the position in the NeoPixel array
        //int pixelIndex = y * matrixWidth + x;
        draw_pixel_raw(imageX, imageY, r,  g,  b);
        //strip.setPixelColor(pixelIndex, strip.Color(r, g, b));
      } else {
        // If the coordinates are outside the image, turn off the LED

         printf("Fehler: Pixelkoordinaten (x %d, y %d) liegen außerhalb der Grenzen!\n", x, y); 
        //int pixelIndex = y * matrixWidth + x;
        //strip.setPixelColor(pixelIndex, strip.Color(0, 0, 0));
        return;
      }
    }
  }

  // Update the NeoPixel strip
  strip.show();
}

void drawHomeInsideSymbol(const unsigned int x, const unsigned int y) {
  drawImage(homeInside10x11, 10, 11, x, y);
}
void drawTemperatureInsideSymbol(const unsigned int x, const unsigned int y) {
  drawImage(temperatureInside5x9, 5, 9, x, y);
}











#endif /* SYMBOLS_H_ */