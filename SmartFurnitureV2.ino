#define AHT_SENSOR 1
#define SGP_SENSOR 1

#include "WiFi.h"
#include "time.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"

uint8_t time_day_start = 6;
uint8_t time_day_end = 23;
uint8_t display_update = 10;  // in sec


#define line_1_start_y 1  //1 raw
#define line_offset_y 6

bool day_activated = true;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);  //start UART
  Serial.println("Serial started");
  ini_ws2812b();
  delay(10);
#if AHT_SENSOR
  ini_aht();
#endif
#if SGP_SENSOR
  ini_sgp();
#endif
  ini_wifi();
  ini_time();
  ini_buttons();
  Serial.println("init finish");
  push_all_values();
}
void update_day() {
#if AHT_SENSOR
  draw_temperature_aht(get_temperature_aht(), 2, line_1_start_y);
  draw_point(13, line_1_start_y);
  draw_humidity_aht(get_humidity_aht(), 2, line_1_start_y + line_offset_y);
  draw_char('%', 10, line_1_start_y + line_offset_y, 255, 255, 90);
#endif
#if SGP_SENSOR
  draw_uint_sgp30(get_co2_sgp(), 1, line_1_start_y + line_offset_y * 2);
#endif
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_offset_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_offset_y * 3);
  draw_mday_esp(get_mday(), 1, line_1_start_y + line_offset_y * 4);
  draw_point(8, line_1_start_y + line_offset_y * 4 + 4);
  draw_mon_esp(get_mon(), 9, line_1_start_y + line_offset_y * 4);
  draw_temperature_esp(get_temperature_esp(), 2, line_1_start_y + line_offset_y * 5);
  draw_point(13, line_1_start_y + line_offset_y * 5);
  draw_humidity_esp(get_humidity_esp(), 2, line_1_start_y + line_offset_y * 6);
  draw_char('%', 10, line_1_start_y + line_offset_y * 6, 255, 255, 90);
  read_brightness();
}
void update_night() {
  clean_line(line_1_start_y);
  clean_line(line_1_start_y + line_offset_y);
#if SGP_SENSOR
  draw_uint_sgp30(get_co2_sgp(), 1, line_1_start_y + line_offset_y * 2);
#endif
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_offset_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_offset_y * 3);
  clean_line(line_1_start_y + line_offset_y * 4);
  clean_line(line_1_start_y + line_offset_y * 5);
  clean_line(line_1_start_y + line_offset_y * 6);
  set_brightness_1();
}

void refresh_all_data() {  //1 sec
#if AHT_SENSOR
  read_aht();
#endif
#if SGP_SENSOR
  read_sgp();
#endif
  //check_wifi();
  read_time();
  read_wetter_data();
  print_time_colon(7, line_1_start_y + line_offset_y * 3);
  static uint16_t cnt = 0;
  cnt++;
  if (cnt > display_update) {
    cnt = 0;
    if (get_hour() > time_day_start && get_hour() < time_day_end) {
      update_day();
      day_activated = true;
    } else {
      if (key_handler() == false) {
        update_night();
        day_activated = false;
      } else if (key_handler() == true) {
        update_day();
        day_activated = true;
      }
    }
  }
}
void loop() {
  unsigned long currentMillis = millis();
  //uart_menu_char();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    //debug_uart();
    refresh_all_data();
  }
}
void debug_uart() {
  debug_uart_aht();
  debug_btn();
  debug_uart_sgp();
  debug_temt6000();
  printLocalTime();
  debug_uart_esp();
}
void push_all_values() {
  read_aht();
  read_sgp();
  read_time();
  read_wetter_data();
  draw_point(13, line_1_start_y);
  for (float i = -9.9; i < get_temperature_aht(); i += 5) {
    draw_temperature_aht(i, 2, line_1_start_y);
  }
  draw_temperature_aht(get_temperature_aht(), 2, line_1_start_y);
  draw_char('%', 10, line_1_start_y + line_offset_y, 255, 255, 90);
  for (uint8_t i = 0; i < get_humidity_aht(); i += 10) {
    draw_humidity_aht(i, 2, line_1_start_y + line_offset_y);
  }
  draw_humidity_aht(get_humidity_aht(), 2, line_1_start_y + line_offset_y);
  for (uint16_t i = 0; i < 401; i += 50) {
    draw_uint_sgp30(i, 1, line_1_start_y + line_offset_y * 2);
  }
  for (uint8_t i = 0; i < get_hour(); i += 5) {
    draw_hour_esp(i, 1, line_1_start_y + line_offset_y * 3);
  }
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_offset_y * 3);
  draw_point(8, line_1_start_y + line_offset_y * 4 + 4);
  for (uint8_t i = 0; i < get_min(); i += 10) {
    draw_min_esp(i, 8, line_1_start_y + line_offset_y * 3);
  }
  draw_min_esp(get_min(), 8, line_1_start_y + line_offset_y * 3);

  for (uint8_t i = 0; i < get_mday(); i += 5) {
    draw_mday_esp(i, 1, line_1_start_y + line_offset_y * 4);
  }
  draw_mday_esp(get_mday(), 1, line_1_start_y + line_offset_y * 4);

  for (uint8_t i = 0; i < get_mon(); i += 2) {
    draw_mon_esp(i, 9, line_1_start_y + line_offset_y * 4);
  }
  draw_mon_esp(get_mon(), 9, line_1_start_y + line_offset_y * 4);

  draw_point(13, line_1_start_y + line_offset_y * 5);
  for (float i = -9.9; i < get_temperature_esp(); i += 5) {
    draw_temperature_esp(i, 2, line_1_start_y + line_offset_y * 5);
  }
  draw_temperature_esp(get_temperature_esp(), 2, line_1_start_y + line_offset_y * 5);

  draw_char('%', 10, line_1_start_y + line_offset_y * 6, 255, 255, 90);
  for (uint8_t i = 0; i < get_humidity_esp(); i += 10) {
    draw_humidity_esp(i, 2, line_1_start_y + line_offset_y * 6);
  }
  draw_humidity_esp(get_humidity_esp(), 2, line_1_start_y + line_offset_y * 6);
}
void uart_menu_char() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    switch (receivedChar) {
      case '?':
        Serial.println("Hello, to set or check values please enter");
        Serial.println("'o' check time offset in sec");
        Serial.println("'s' set sommer time offset(3600s)");
        Serial.println("'w' set winter time offset(0s)");
        Serial.println("'a' refresh CO2 sensor");
        Serial.println("'h' check aht temperature sensor");
        Serial.println("'g' check co2/voc sensor");
        Serial.println("'l' check light sensor");
        Serial.println("'t' check time");
        Serial.println("'e' check weather data");
        break;
      case 'o':
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        break;
      case 's':
        set_gmt_offset(3600);
        Serial.println("now is sommer time !");
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        ini_time();
        get_hour();
        break;
      case 'w':
        set_gmt_offset(0);
        Serial.println("now is winter time !");
        Serial.print("Offset in sec now: ");
        Serial.println(get_gmt_offset());
        ini_time();
        get_hour();
        break;
      case 'a':
        ini_sgp();
        Serial.print("CO2 ppn is now: ");
        Serial.println(get_co2_sgp());
        break;
      case 'h':
        debug_uart_aht();
        break;
      case 'g':
        debug_uart_sgp();
        break;
      case 'l':
        debug_temt6000();
        break;
      case 't':
        printLocalTime();
        break;
      case 'e':
        read_wetter_data_all();
        break;
    }
  }
}

String receivedMessage;

void read_uart() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);  // Print the received message in the Serial monitor
      //receivedMessage = "";  // Reset the received message
    } else {
      receivedMessage += receivedChar;  // Append characters to the received message
    }
  }
}
