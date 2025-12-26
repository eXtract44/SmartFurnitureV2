#if AHT_SENSOR 
#include <Adafruit_AHT10.h>

#define SDA_PIN_AHT10 33
#define SCL_PIN_AHT10 32

Adafruit_AHT10 aht;  //constructor
sensors_event_t aht_10_humidity, aht_10_temperature;

void ini_aht() {
  Wire1.begin(SDA_PIN_AHT10, SCL_PIN_AHT10);
  if (!aht.begin(&Wire1)) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
}
void read_aht() {
  aht.getEvent(&aht_10_humidity, &aht_10_temperature);  // populate temp and humidity objects with fresh data
}
float get_temperature_aht(){
 return static_cast<float>(aht_10_temperature.temperature);
}
uint8_t get_humidity_aht(){
  return static_cast<uint8_t>(aht_10_humidity.relative_humidity);
}
void debug_uart_aht(){
  Serial.print("Temperature: "); Serial.print(aht_10_temperature.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(aht_10_humidity.relative_humidity); Serial.println("% rH");
}
#endif