#include <config_.h>



void setup() {
  Serial.begin(115200);
}



int i = 1;



void loop() {
i++;
if(i>14)i=1;
setUartPixel(1, 1);
  delay(3000);
  printUartPixelArray();
}