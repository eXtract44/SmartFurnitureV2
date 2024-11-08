#ifndef FONTS_H_
#define FONTS_H_
uint8_t font3x5[][5] = {
  { 0b111,
    0b101,
    0b111,
    0b101,
    0b101 },  // A
  {
    0b110,
    0b101,
    0b110,
    0b101,
    0b110 },  // B
  {
    0b111,
    0b100,
    0b100,
    0b100,
    0b111 },  // C
  {
    0b110,
    0b101,
    0b101,
    0b101,
    0b110 },  // D
  { 0b111,
    0b100,
    0b111,
    0b100,
    0b111 },  // E
  { 0b111,
    0b100,
    0b111,
    0b100,
    0b100 },  // F
  { 0b111,
    0b100,
    0b101,
    0b101,
    0b111 },  // G
  { 0b101,
    0b101,
    0b111,
    0b101,
    0b101 },  // H
  { 0b111,
    0b010,
    0b010,
    0b010,
    0b111 },  // I
  { 0b001,
    0b001,
    0b001,
    0b101,
    0b111 },  // J
  { 0b101,
    0b101,
    0b110,
    0b101,
    0b101 },  // K
  { 0b100,
    0b100,
    0b100,
    0b100,
    0b111 },  // L
  { 0b101,
    0b111,
    0b111,
    0b101,
    0b101 },  // M
  { 0b111,
    0b101,
    0b101,
    0b101,
    0b101 },  // N
  { 0b111,
    0b101,
    0b101,
    0b101,
    0b111 },  // O
  { 0b111,
    0b101,
    0b111,
    0b100,
    0b100 },  // P
  { 0b111,
    0b101,
    0b101,
    0b111,
    0b001 },  // Q
  { 0b111,
    0b101,
    0b111,
    0b110,
    0b101 },  // R
  { 0b111,
    0b100,
    0b111,
    0b001,
    0b111 },  // S
  { 0b111,
    0b010,
    0b010,
    0b010,
    0b010 },  // T
  { 0b101,
    0b101,
    0b101,
    0b101,
    0b111 },  // U
  { 0b101,
    0b101,
    0b101,
    0b101,
    0b010 },  // V
  { 0b101,
    0b101,
    0b111,
    0b111,
    0b101 },  // W
  { 0b101,
    0b101,
    0b010,
    0b101,
    0b101 },  // X
  { 0b101,
    0b101,
    0b010,
    0b010,
    0b010 },  // Y
  { 0b111,
    0b001,
    0b010,
    0b100,
    0b111 },  // Z
  { 0b111,
    0b101,
    0b101,
    0b101,
    0b111 },  // 0
  { 0b110,
    0b010,
    0b010,
    0b010,
    0b010 },  // 1
  { 0b111,
    0b001,
    0b111,
    0b100,
    0b111 },  // 2
  { 0b111,
    0b001,
    0b111,
    0b001,
    0b111 },  // 3
  { 0b101,
    0b101,
    0b111,
    0b001,
    0b001 },  // 4
  { 0b111,
    0b100,
    0b111,
    0b001,
    0b111 },  // 5
  { 0b111,
    0b100,
    0b111,
    0b101,
    0b111 },  // 6
  { 0b111,
    0b001,
    0b010,
    0b010,
    0b010 },  // 7
  { 0b111,
    0b101,
    0b111,
    0b101,
    0b111 },  // 8
  { 0b111,
    0b101,
    0b111,
    0b001,
    0b111 },  // 9
};

const uint8_t font4x9[10][9] = {
  // Number 0
  {0b0110,  // Row 1
   0b1001,  // Row 2
   0b1001,  // Row 3
   0b1001,  // Row 4
   0b1001,  // Row 5
   0b1001,  // Row 6
   0b1001,  // Row 7
   0b1001,  // Row 8
   0b0110}, // Row 9

  // Number 1
  {0b0010,  // Row 1
   0b0110,  // Row 2
   0b1010,  // Row 3
   0b0010,  // Row 4
   0b0010,  // Row 5
   0b0010,  // Row 6
   0b0010,  // Row 7
   0b0010,  // Row 8
   0b1111}, // Row 9

  // Number 2
  {0b0110,
   0b1001,
   0b0001,
   0b0010,
   0b0100,
   0b1000,
   0b1000,
   0b1001,
   0b1111},

  // Number 3
  {0b0110,
   0b1001,
   0b0001,
   0b0010,
   0b0110,
   0b0001,
   0b0001,
   0b1001,
   0b0110},

  // Number 4
  {0b0001,
   0b0011,
   0b0101,
   0b1001,
   0b1111,
   0b0001,
   0b0001,
   0b0001,
   0b0001},

  // Number 5
  {0b1111,
   0b1000,
   0b1000,
   0b1110,
   0b0001,
   0b0001,
   0b0001,
   0b1001,
   0b0110},

  // Number 6
  {0b0110,
   0b1001,
   0b1000,
   0b1110,
   0b1001,
   0b1001,
   0b1001,
   0b1001,
   0b0110},

  // Number 7
  {0b1111,
   0b0001,
   0b0001,
   0b0010,
   0b0010,
   0b0100,
   0b0100,
   0b1000,
   0b1000},

  // Number 8
  {0b0110,
   0b1001,
   0b1001,
   0b0110,
   0b1001,
   0b1001,
   0b1001,
   0b1001,
   0b0110},

  // Number 9
  {0b0110,
   0b1001,
   0b1001,
   0b1001,
   0b0111,
   0b0001,
   0b0001,
   0b1001,
   0b0110}
};

void drawNumber4x9(int number, int startX, int startY, uint8_t r, uint8_t g, uint8_t b) {
  if (number < 0 || number > 9) return;  // Validate the number

  for (int row = 0; row < 9; row++) {
    uint8_t rowData = font4x9[number][row];  // Get the row data for the number

    for (int col = 0; col < 4; col++) {
      if (rowData & (1 << (3 - col))) {  // Check if the bit is set
        int pixelX = startX + col;
        int pixelY = startY + row;

        // Check if the pixel is within the matrix boundaries
        if (pixelX >= 0 && pixelX < LED_DISPLAY_LENGTH && pixelY >= 0 && pixelY < LED_DISPLAY_HEIGHT) {
          int pixelIndex = pixelY * LED_DISPLAY_LENGTH + pixelX;
          strip.setPixelColor(pixelIndex, strip.Color(r, g, b));
        }
      }
    }
  }

  // Update the NeoPixel strip
  strip.show();
}




#endif /*FONTS_H_*/