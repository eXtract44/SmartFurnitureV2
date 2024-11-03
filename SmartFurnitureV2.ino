#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHT10.h>
#include "Adafruit_SGP30.h"
#include <Wire.h>
#include <config_.h>

int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel strip(LED_DISPLAY_COUNT_LED, LED_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
}


void loop() {
  setText3x5("24.4*", 1, LED_DISPLAY_LINE_1);
  setText3x5("56 %", 1, LED_DISPLAY_LINE_2);
  setText3x5("999", 1, LED_DISPLAY_LINE_3);
  setText3x5("23:45", 1, LED_DISPLAY_LINE_4);
  setText3x5("30.12", 1, LED_DISPLAY_LINE_5);
  setText3x5("14.3*", 1, LED_DISPLAY_LINE_6);
  setText3x5("45  %", 1, LED_DISPLAY_LINE_7);
  printPixelArray();
  delay(3000);
  setAllpixelOFF();
}