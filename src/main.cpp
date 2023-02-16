#include <Adafruit_NeoPixel.h>
 
#define LED_PIN 22
#define NUM_LEDS 256


 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

String text = "Hello World";
 
void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(15);
}
 
void loop() {
  // Loop through each character in the text
  for (int i = 0; i < text.length(); i++) {
    // Get the ASCII code for the character
    int ascii = text.charAt(i);
    
    // Map the ASCII code to a color value
    int color = map(ascii, 32, 126, 0, 255);
    
    // Set the Neopixels to the color
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels.setPixelColor(j, pixels.Color(color, color, color));
    }
    
    // Update the Neopixels
    pixels.show();
    
    // Wait for a short time
    delay(100);
  }
}

void displayText(String text) {
  // Loop through each character in the text
  for (int i = 0; i < text.length(); i++) {
    // Get the ASCII code for the character
    int ascii = text.charAt(i);
    
    // Map the ASCII code to a color value
    int color = map(ascii, 32, 126, 0, 255);
    
    // Set the Neopixels to the color
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels.setPixelColor(j, pixels.Color(color, color, color));
    }
    
    // Update the Neopixels
    pixels.show();
    
    // Wait for a short time
    delay(2000);
  }
}