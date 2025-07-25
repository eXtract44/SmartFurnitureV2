#include <Wire.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp_30;

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
