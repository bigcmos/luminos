#include <Adafruit_NeoPixel.h>

#include "luminos_util.h"

#define PIN 6


//RGBColor RGB_ORANGE = {160, 40, 0};
//RGBColor RGB_GREEN = {128, 255, 0};
//RGBColor RGB_PURPLE = {150, 0, 150};
//RGBColor RGB_RED = {255, 0, 0};

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

RGBColor PRIMARY = RGB_RED;
RGBColor BACKGROUND = RGB_GREEN;
RGBColor ACCENT = RGB_BLUE;

uint32_t RGBToColor(RGBColor color) {
  return strip.Color(color.r, color.g, color.b);
}

void loop() {
  colorWipe(RGBToColor(RGB_WHITE), 100);
  delay(1000);
  colorWipe(RGBToColor(PRIMARY), 100);
  delay(1000);
  colorWipe(RGBToColor(BACKGROUND), 100);
  delay(1000);
  colorWipe(RGBToColor(ACCENT), 100);
  delay(1000);

  for (int i = 0; i < 5; ++i) {
    colorFade(PRIMARY, BACKGROUND, 20);
    colorFade(BACKGROUND, PRIMARY, 20);
  }

  colorWipe(RGBToColor(RGB_WHITE), 100);
  delay(10000);
  colorWipe(RGBToColor(ACCENT), 100);
  delay(10000);
  colorWipe(RGBToColor(PRIMARY), 100);
  delay(10000);
  colorWipe(RGBToColor(BACKGROUND), 100);
  delay(10000);

  // Some example procedures showing how to display to the pixels:
  for (uint8_t i = 0; i < 10; ++i) {
    colorWipe(RGBToColor(PRIMARY), 100);
    colorWipe(RGBToColor(BACKGROUND), 100);
  }

  for (uint8_t i = 0; i < 10; ++i) {
    inverseColorWipe(RGBToColor(PRIMARY), 50);
    colorWipe(RGBToColor(BACKGROUND), 50);
  }

  // Send a theater pixel chase in...
  theaterChase(RGBToColor(PRIMARY), 1000);
  theaterChase(RGBToColor(BACKGROUND), 1000);

  theaterChaceWithBackground(RGBToColor(PRIMARY), RGBToColor(BACKGROUND), 1000);

  //rainbow(20);
  //rainbowCycle(20);
  //theaterChaseRainbow(50);
}

void colorFade(struct RGBColor primary, struct RGBColor background, uint32_t delay_ms) {
  uint8_t MAX_STEPS = 255;
  for (uint8_t steps = 0; steps < MAX_STEPS; ++steps) {
      RGBColor tmp;
      tmp.r = background.r - (uint8_t)(((background.r - primary.r) * steps) / MAX_STEPS);
      tmp.g = background.g - (uint8_t)(((background.g - primary.g) * steps) / MAX_STEPS);
      tmp.b = background.b - (uint8_t)(((background.b - primary.b) * steps) / MAX_STEPS);
      uint32_t c = RGBToColor(tmp);
    for (uint16_t i = 0; i < strip.numPixels(); ++i) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(delay_ms);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void inverseColorWipe(uint32_t c, uint8_t wait) {
  for (int16_t i = strip.numPixels() - 1; i >= 0; --i) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaceWithBackground(uint32_t c, uint32_t b, uint8_t wait) {
  for (int i=0; i<10; i++) {  //do 10 cycles of chasing
    for (int j=0; j < strip.numPixels(); j++) {
      if ((i + j) % 3 == 0) {
        strip.setPixelColor(j, c);
      } else {
        strip.setPixelColor(j, b);
      }
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

