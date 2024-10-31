

void printFullDisplay(){
  
}
uint8_t uartPixelArray[LED_DISPLAY_LENGTH+1][LED_DISPLAY_HEIGHT+1];

void setUartPixel(unsigned int x,unsigned int y){
     if (x > LED_DISPLAY_LENGTH || y > LED_DISPLAY_HEIGHT) {
        Serial.println("Koordinaten außerhalb des Bildbereichs!");
        return;
    }
uartPixelArray[y][x] = '1';
}

void printUartPixelArray(){
  String output = "";
  unsigned int i,j;
     for ( i = 1; i <= LED_DISPLAY_HEIGHT; i++) {
        for ( j = 1; j <= LED_DISPLAY_LENGTH; j++) {
          if(pixelArray[i][j] == '1'){
            output += String(pixelArray[i][j]);
          }else{
            
          }
          
        }
        output += "\n";
    }
   Serial.println();
}