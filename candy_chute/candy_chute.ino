/**
 * 
 *  This is the light strip used to wrap around the chute
 *  for dispensing Candy during Halloween 2020
 * 
 */
#include <Adafruit_NeoPixel.h>

#define PIN 5

#define LED_COUNT 300
#define RAINBOW_CYCLES 2
#define THEATER_CHASE_CYCLES 3

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  uint8_t speed = 30; // This is the global speed factor.
  // The lower the speed, the faster the lights will become.

  lavaCycle(speed);
  rainbowCycle(speed);
  theaterChaseRainbow(speed);
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*RAINBOW_CYCLES; j++) {
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

/*
 * Lava cycle is a lot like the theater chase but only goes through the lava colors
 * i.e. Red to Yellow with 
 */
void lavaCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*RAINBOW_CYCLES; j++) {
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, lavaWheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}



//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < THEATER_CHASE_CYCLES; q++) {
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


/*
 * pass a wheel position (0 to 255) and this will generate a color value in the Red and Yellow range
 *  - a lava color
 */
uint32_t lavaWheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 255 - WheelPos * 3, 0);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 0, 0);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
