#define COLOR_TIME_COLON 255, 255, 0
#define COLOR_TIME_COLON_BLINK 255, 255, 255

const char* ntpServer = "pool.ntp.org";  // time
long gmtOffset_sec = 3600;               //3600 sommer / 0 winter   ,,,,,,,,,ab 26.03 bis 29.10 - 3600 time
const int daylightOffset_sec = 3600;     //time

struct tm timeinfo;

int get_last_sunday(int year, int month) {
  struct tm t = {};
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = 31;
  mktime(&t);
  // смещение назад до последнего воскресенья
  t.tm_mday -= t.tm_wday;
  mktime(&t);
  return t.tm_mday;
}

int get_dst_offset() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  int year = timeinfo.tm_year + 1900;
  int mon = timeinfo.tm_mon;  // 0-11
  int mday = timeinfo.tm_mday;
  int hour = timeinfo.tm_hour;

  int last_sun_march = get_last_sunday(year, 2);  // март
  int last_sun_oct   = get_last_sunday(year, 9);  // октябрь

  if (
    (mon > 2 && mon < 9) ||  // апрель–сентябрь → CEST
    (mon == 2 && (mday > last_sun_march || (mday == last_sun_march && hour >= 2))) ||
    (mon == 9 && (mday < last_sun_oct || (mday == last_sun_oct && hour < 3)))
  ) {
    return 0;  // CEST: UTC+2  //sommer time
  }
  return -1;    // CET: UTC+1   //winter time
}

void set_gmt_offset(long offset){
  gmtOffset_sec = offset; 
}
long get_gmt_offset(){
  return gmtOffset_sec; 
}
void ini_time() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void read_time() {
  getLocalTime(&timeinfo);
}
uint8_t get_hour() {
  return timeinfo.tm_hour+get_dst_offset();
}
uint8_t get_min() {
  return timeinfo.tm_min;
}
uint8_t get_mday() {
  return timeinfo.tm_mday;
}
uint8_t get_mon() {
  return timeinfo.tm_mon + 1;
}
uint8_t get_sec() {
  return timeinfo.tm_sec;
}

void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
void print_time_colon(const uint8_t x, const uint8_t y) {
  static uint8_t cnt = 0;
  cnt++;
  if (cnt == 1) {
    draw_pixel_now(x, y + 1, COLOR_TIME_COLON);
    draw_pixel_now(x, y + 3, COLOR_TIME_COLON);
  }
  if (cnt == 2) {
    draw_pixel_now(x, y + 1, COLOR_TIME_COLON_BLINK);  //1, 19
    draw_pixel_now(x, y + 3, COLOR_TIME_COLON_BLINK);
    cnt = 0;
  }
}
