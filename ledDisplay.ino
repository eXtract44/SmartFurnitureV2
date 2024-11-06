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



















































#endif
