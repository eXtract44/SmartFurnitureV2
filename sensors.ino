
#include <config_.h>

Adafruit_SGP30 sgp_30;
Adafruit_AHT10 aht;  
sensors_event_t aht_10_humidity, aht_10_temperature;
///////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////


void ini_sgp() {
  Wire.begin();

  if (!sgp_30.begin()) {
    Serial.println("Sensor not found :(");
    while (1);     
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp_30.serialnumber[0], HEX);
  Serial.print(sgp_30.serialnumber[1], HEX);
  Serial.println(sgp_30.serialnumber[2], HEX);
    // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
}

uint32_t getAbsoluteHumidity(const float temperature, const float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature));  // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                 // [mg/m^3]
  return absoluteHumidityScaled;
}

void read_sgp() {
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  //float temperature = 22.1; // [°C]
  //float humidity = 45.2; // [%RH]
  sgp_30.setHumidity(getAbsoluteHumidity(get_temperature_aht(), static_cast<float>(get_humidity_aht())));

  if (!sgp_30.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
}
uint16_t get_co2_sgp(){
return sgp_30.eCO2;
}

void debug_uart_sgp() {
  //static auto counter = 0;
  Serial.print("TVOC ");
  Serial.print(sgp_30.TVOC);
  Serial.print(" ppb\t");
  Serial.print("eCO2 ");
  Serial.print(sgp_30.eCO2);
  Serial.println(" ppm");

  if (!sgp_30.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 ");
  Serial.print(sgp_30.rawH2);
  Serial.print(" \t");
  Serial.print("Raw Ethanol ");
  Serial.print(sgp_30.rawEthanol);
  Serial.println("");
  //counter++;
  //if (counter > 30) {
    //counter = 0;
    uint16_t TVOC_base, eCO2_base;
    if (!sgp_30.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x");
    Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x");
    Serial.println(TVOC_base, HEX);
  //}
}
////////////////////////////////////////////////////////////////////////
uint16_t brightness = 0;
void read_brightness() {
  static uint16_t cnt = 0;
  cnt++;
  if(cnt > 60){
  cnt= 0; 
  brightness = map(analogRead(LIGHT_SENSOR_PIN), 0, 4095, 1, 40); 

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
