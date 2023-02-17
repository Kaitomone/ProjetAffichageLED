#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/Picopixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    22

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 1024

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 32, LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


// Declare our NeoPixel strip object:
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  // put your setup code here, to run once:
  void drawPixel(uint16_t x, uint16_t y, uint16_t color);
  void drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
  void drawFastHLine(uint8_t x0, uint8_t y0, uint8_t length, uint16_t color);
  
  void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);

  void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

  void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
  void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);

  void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  
  void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
  
  void setCursor(int16_t x0, int16_t y0);
  void setTextColor(uint16_t color);
  void setTextColor(uint16_t color, uint16_t backgroundcolor);
  void setTextSize(uint8_t size);
  void setTextWrap(boolean w);
  
  matrix.begin();

  matrix.setBrightness(5);
  matrix.setTextWrap(false);

    
  
  matrix.setFont(&Picopixel);
  matrix.setCursor(0, 4);
  matrix.setTextSize(1);
  matrix.show();

}

void loop() {
  
  matrix.clear();

  matrix.setCursor(0, 4);

  matrix.setTextSize(1);
  matrix.setTextColor(WHITE);
  matrix.print("ENZO");

  matrix.show();

  matrix.print('\n');

  matrix.setTextColor(RED);

  matrix.print("SAMY");

  matrix.show();

  delay(5000);

  matrix.clear();

  matrix.setCursor(0, 4);
  
  matrix.drawCircle(8, 8, 7, RED);

  matrix.show();

  delay(5000);

  matrix.clear();

  for (int i = 0; i < LED_COUNT; i++) {
    matrix.setPixelColor(i, matrix.Color(255, 255, 255));
  }
  
  // Update the Neopixels
  matrix.show();
  
  // Wait for a second
  delay(5000);
}