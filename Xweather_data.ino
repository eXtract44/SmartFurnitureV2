const String openWeatherMapApiKey = "467236d17fdfc652af154eb837422f8d";
String city = "Dortmund";
String countryCode = "DE";

String jsonBuffer;
struct esp {
  double temp;
  int humidity;
};
esp esp_data;

#define UPADTE_TIME_WEATHER 900  // in sec
void read_wetter_data() {
  static uint16_t cnt = UPADTE_TIME_WEATHER + 1;
  cnt++;
  if (cnt > UPADTE_TIME_WEATHER) {  // 15 min
    cnt = 0;
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    jsonBuffer = httpGETRequest(serverPath.c_str());
    //Serial.println(jsonBuffer);
    JSONVar myObject = JSON.parse(jsonBuffer);
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    esp_data.temp = double(myObject["main"]["temp"]) - 273;
    esp_data.humidity = int(myObject["main"]["humidity"]);
  }
}
void read_wetter_data_all() {
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    jsonBuffer = httpGETRequest(serverPath.c_str());
    Serial.println(jsonBuffer);
}
float get_temperature_esp() {
  return static_cast<float>(esp_data.temp);
}
uint8_t get_humidity_esp() {
  return static_cast<uint8_t>(esp_data.humidity);
}

void debug_uart_esp() {
  Serial.print("Temperature ESP: ");
  Serial.print(get_temperature_esp());
  Serial.println(" degrees C");
  Serial.print("Humidity ESP: ");
  Serial.print(get_humidity_esp());
  Serial.println("% rH");
}
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  String payload = "{}";
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    payload = http.getString();
  } else {

    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
