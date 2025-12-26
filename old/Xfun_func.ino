
/*
#define swap(a,b) {int16_t t=a;a=b;b=t;}


void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b) {
  int step = abs(y2 - y1) > abs(x2 - x1);
  if (step)
  {
    swap(x1, y1);
    swap(x2, y2);
  }
  if (x1 > x2)
  {
    swap(x1, x2);
    swap(y1, y2);
  }
  int dx, dy;
  dx = x2 - x1;
  dy = abs(y2 - y1);
  int err = dx / 2;
  int ystep;
  if (y1 < y2) ystep = 1;
  else ystep = -1;
  for (; x1 <= x2; x1++)
  {
    if (step) draw_pixel_now(y1, x1, r, g, b);
    else draw_pixel_now(x1, y1, r, g, b);
    err -= dy;
    if (err < 0)
    {
      y1 += ystep;
      err += dx;
    }
  }
  strip.show();
}
void draw_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t g, uint8_t b) {
  draw_line(x1, y1, x2,  y1, r, g, b);
  draw_line(x2, y1, x2,  y2, r, g, b);
  draw_line(x1, y1, x1,  y2, r, g, b);
  draw_line(x1, y2, x2,  y2, r, g, b);
}
void check_resolution() {
  draw_line(1, 1, 1, strip_high, COLOR_RED);
  delay(300);
  draw_line(1, strip_high, strip_len, strip_high, COLOR_RED);
  delay(300);
  draw_line(strip_len, strip_high, strip_len, 1, COLOR_RED);
  delay(300);
  draw_line(strip_len, 1, 1, 1, COLOR_RED);
  delay(300);
  for (uint16_t f = 0; f <= strip_high - 3; f++) {
    draw_line(2, f + 2, strip_len - 1, 2 + f, COLOR_WHITE);
    delay(300);
  }

}
void draw_fire_hor() {
  uint8_t rand_x = random(1, strip_len + 1);
  uint8_t rand_y = random(1, strip_high - 6);
  draw_line(rand_x, 1, rand_x, strip_high, 0, 0, 0);
  draw_line(rand_x, strip_high, rand_x, rand_y, 128, 17, 0);
  draw_pixel_now(rand_x, rand_y + 5, 250, 152, 0); //1
  draw_pixel_now(rand_x, rand_y + 4, 255, 117, 0); //2
  draw_pixel_now(rand_x, rand_y + 3, 252, 100, 0); //3
  draw_pixel_now(rand_x, rand_y + 2, 215, 53, 2); //4
  draw_pixel_now(rand_x, rand_y + 1, 182, 34, 3); //5
}
void draw_fire_ver() {
  uint8_t rand_x = random(1, 5);
  uint8_t rand_y = random(1, 21);
  draw_line(1, rand_y, 7, rand_y, 0, 0, 0);
  draw_line(1, rand_y, rand_x, rand_y, 128, 17, 0);
  draw_pixel_now(rand_x + 3, rand_y, 250, 152, 0); //1
  //draw_pixel_now(rand_x, rand_y+4, 255, 117, 0); //2
  draw_pixel_now(rand_x + 2, rand_y, 252, 100, 0); //3
  //draw_pixel_now(rand_x, rand_y+2, 215, 53, 2); //4
  draw_pixel_now(rand_x + 1, rand_y, 182, 34, 3); //5
}
void while_() {
  draw_fire_ver();

  //check_resolution();
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < COUNT_LED; i++ ) {
    setPixel(i, red, green, blue);
  }
  strip.show();
}
#define LED_COUNT_FIRE 41
void Fire_ver(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT_FIRE];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < LED_COUNT_FIRE; i++) { //LED_COUNT
    cooldown = random(0, ((Cooling * 10) / LED_COUNT_FIRE) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = LED_COUNT_FIRE - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);

  }
  int r = random(1, strip_len + 1);
  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < LED_COUNT_FIRE; j++) {
    setPixelHeatColor(j, heat[j], r);
  }

  strip.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature, uint8_t x) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    draw_pixel_now(x, strip_high - Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    draw_pixel_now(x, strip_high - Pixel, 255, heatramp, 0);
  } else {                               // coolest
    draw_pixel_now(x, strip_high - Pixel, heatramp, 0, 0);
  }
}
#define LED_COUNT_METEOR 41
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay, uint8_t x) {
  setAll(0, 0, 0);
  for (int i = 0; i < LED_COUNT_METEOR + LED_COUNT_METEOR; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < LED_COUNT_METEOR; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack(j, meteorTrailDecay , x);
      }
    }
    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < LED_COUNT_METEOR) && (i - j >= 0) ) {
        // draw_pixel_now(x, LED_COUNT_METEOR-i-j, red, green, blue);
        draw_pixel_now(x, i - j, red, green, blue);
        //setPixel(i-j, red, green, blue);
      }
    }
    strip.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue, uint8_t x) {
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  //int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  //strip.setPixelColor(ledNo, r,g,b);
  draw_pixel_now(x, ledNo, r, g, b);
  //draw_pixel_now(x, LED_COUNT_METEOR-ledNo, r,g,b);
}
void idle() {
  setAll(0, 0, 0);
  lamp_on(1, 5);
  //if (analogRead(A0) < 300) {

  //}
  //uint8_t gg = random(1, 15);
  //meteorRain(0xff, 0xff, 0xff, 10, 64, true, 30, gg);
  //Fire_ver(55,120,15);
}
void fire_idle() {

  //uint8_t gg = random(1, 15);
  //meteorRain(0xff, 0xff, 0xff, 10, 64, true, 30, gg);
  Fire_ver(55, 120, 15);
}
void lamp_on(uint8_t x, uint8_t y) {
  uint8_t speed_light = 40;

  for (uint16_t rgb = 0; rgb < 255; rgb = rgb + speed_light) {

    for (uint8_t i = 0; i <= 3; i++) {
      draw_pixel_now(x + 3, y + i + 7, rgb, rgb, rgb);
    }

    for (uint8_t i = 0; i <= 5; i++) {
      draw_pixel_now(x + 4, y + i + 6, rgb, rgb, rgb);
    }

    for (uint8_t i = 0; i <= 7; i++) {
      draw_pixel_now(x + 5, y + i + 5, rgb, rgb, rgb);
    }

    for (uint8_t i = 0; i <= 7; i++) {
      draw_pixel_now(x + 6, y + i + 5, rgb, rgb, rgb);
    }
    for (uint8_t i = 0; i <= 7; i++) {
      draw_pixel_now(x + 7, y + i + 5, rgb, rgb, rgb);
    }
    for (uint8_t i = 0; i <= 7; i++) {
      draw_pixel_now(x + 8, y + i + 5, rgb, rgb, rgb);
    }

    for (uint8_t i = 0; i <= 5; i++) {
      draw_pixel_now(x + 9, y + i + 6, rgb, rgb, rgb);
    }
    for (uint8_t i = 0; i <= 3; i++) {
      draw_pixel_now(x + 10, y + i + 7, rgb, rgb, rgb);
    }


    draw_pixel_now(x + 5, y + 2 , rgb, rgb, rgb);
    draw_pixel_now(x + 5, y + 3 , rgb, rgb, rgb);

    draw_pixel_now(x + 8, y + 2 , rgb, rgb, rgb);
    draw_pixel_now(x + 8, y + 3 , rgb, rgb, rgb);

    draw_pixel_now(x + 2, y + 4 , rgb, rgb, rgb);
    draw_pixel_now(x + 3, y + 5 , rgb, rgb, rgb);

    draw_pixel_now(x, y + 9, rgb, rgb, rgb);
    draw_pixel_now(x + 1, y + 9, rgb, rgb, rgb);

    draw_pixel_now(x + 2, y + 13, rgb, rgb, rgb);
    draw_pixel_now(x + 3, y + 12, rgb, rgb, rgb);

    draw_pixel_now(x + 5, y + 14, rgb, rgb, rgb);
    draw_pixel_now(x + 5, y + 15, rgb, rgb, rgb);

    draw_pixel_now(x + 8, y + 14, rgb, rgb, rgb);
    draw_pixel_now(x    + 8, y + 15, rgb, rgb, rgb);

    draw_pixel_now(x + 10, y + 12, rgb, rgb, rgb);
    draw_pixel_now(x + 11, y + 13, rgb, rgb, rgb);

    draw_pixel_now(x + 12, y + 9, rgb, rgb, rgb);
    draw_pixel_now(x + 13, y + 9, rgb, rgb, rgb);

    draw_pixel_now(x + 10, y + 5, rgb, rgb, rgb);
    draw_pixel_now(x + 11, y + 4, rgb, rgb, rgb);

    strip.show();
  }
  delay(100);

}
*/
