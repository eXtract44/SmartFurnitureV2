#include <config_.h>



void setup() {
  Serial.begin(115200);
}


void loop() {
  for (uint8_t i = 0; i <= 9; i++) {
    setNumber(i, 1, LED_DISPLAY_LINE_1);
    setNumber(i, 1, LED_DISPLAY_LINE_2);
    setNumber(i, 1, LED_DISPLAY_LINE_3);
    setNumber(i, 1, LED_DISPLAY_LINE_4);
    setNumber(i, 1, LED_DISPLAY_LINE_5);
    setNumber(i, 1, LED_DISPLAY_LINE_6);
    setNumber(i, 1, LED_DISPLAY_LINE_7);
    printUartArray();
    delay(3000);
    cleanUartArray();
  }
}