uint16_t brightness = 0;
void read_brightness() {
  static uint16_t cnt = 0;
  cnt++;
  if(cnt > 60){
  cnt= 0; 
  brightness = map(analogRead(A0), 0, 4095, 1, 40); 

  strip.setBrightness(brightness);
  strip.show();
  }
}
void set_brightness_1() {
  strip.setBrightness(1);
}
void debug_temt6000(){
    Serial.print("analogRead(A0) = ");
  Serial.print(analogRead(A0));
  Serial.println(" bit");
  Serial.print("brightness in % = ");
  Serial.println(brightness);
}