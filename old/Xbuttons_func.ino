#define TOUCH_PIN   5

const uint8_t activation_time_key = 3;  // in sec
const uint8_t delay_after_key = 60;  // in sec

void ini_buttons(){
pinMode(TOUCH_PIN, INPUT);
}
bool read_button_1(){
     return digitalRead(TOUCH_PIN);  
}
void debug_btn(){
  if(read_button_1()){
    Serial.println("Btn1 pressed");
  }
}
bool key_handler() {
  static bool key_state = false;
  static uint16_t cnt_key_1, cnt_delay = 0;
 
  if (read_button_1()) {
    cnt_key_1++;
    if (cnt_key_1 > activation_time_key) {//activ time in sec
        key_state= true;
    }
  }else if(key_state == true){
        cnt_delay++;
        if(cnt_delay > delay_after_key){
        cnt_key_1 = 0;
        cnt_delay = 0;
        key_state= false;       
       }
  }
  return key_state;
}
