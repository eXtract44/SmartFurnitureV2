#include "symbols.h"

void drawScreenHome(){
  unsigned int x_home_start = 1;
  unsigned int y_home_start = 1;
  drawHomeInsideSymbol(x_home_start+2, y_home_start);
  drawTemperatureInsideSymbol(x_home_start, y_home_start+11);
}

void drawTemperatureInsideValue(const unsigned int x, const unsigned int y) {
  read_aht();
  float temperature = get_temperature_aht();
  //drawTemperature(x,y,temperature);
}

void drawHumidityInsideSymbol(const unsigned int x, const unsigned int y) {

  //drawHumiditySymbol()
}

void drawHumidityInsideValue(const unsigned int x, const unsigned int y) {
  read_aht();
  float humidity = get_humidity_aht();
  //drawHumidity(x,y,temperature);
}
void drawAirQualityInsideSymbol(const unsigned int x, const unsigned int y) {

  //drawAirQualitySymbol()_-------------------------
}

void drawAirQualityInsideValue(const unsigned int x, const unsigned int y) {
  read_sgp();
  uint16_t airQuality = get_co2_sgp();
  airQuality = map(airQuality, 400, 2400, 99, 0);
  airQuality = constrain(airQuality, 0, 99);
  //drawairQuality(x,y,temperature);
}
