#include <config_.h>



void setup() {
  Serial.begin(115200);
}


void loop() {
  
displayText3x5("24.4*", 1, LED_DISPLAY_LINE_1, 0xffffffff);
displayText3x5("56%", 1, LED_DISPLAY_LINE_2, 0xffffffff);
displayText3x5("9012", 1, LED_DISPLAY_LINE_3, 0xffffffff);
displayText3x5("13:45", 1, LED_DISPLAY_LINE_4, 0xffffffff);
displayText3x5("07.12", 1, LED_DISPLAY_LINE_5, 0xffffffff);
displayText3x5("14.3*", 1, LED_DISPLAY_LINE_6, 0xffffffff);
displayText3x5("45", 1, LED_DISPLAY_LINE_7, 0xffffffff);
printUartArray();
    delay(3000);
    cleanUartArray();
  
}