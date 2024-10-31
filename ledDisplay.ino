#if LED_DISPLAY
#include <Adafruit_NeoPixel.h>
#include <config_.h>




int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel strip(LED_DISPLAY_COUNT_LED, LED_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);


void ledDisplayInit() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(1);  // Set BRIGHTNESS to about 1/5 (max = 255)
}



















































#endif
