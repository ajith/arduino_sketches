/**

    This is a chrismas star, made with 5 strips of 15 LEDs on each.
    Many of the patterns are from the candy chute (and they look pretty good
    when arranged like a star)
    Some patterns are specifically written for the star.

*/
#include <Adafruit_NeoPixel.h>

#define PIN 5

#define LED_COUNT 75

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

  starRainbow(speed, false); //light up the points only
  starRainbowChasing(speed, false); //points chase
  starRainbowChasing(speed, true); //center chase
  rainbowCycle(speed); //full rainbow
  theaterChaseRainbow(speed); //full star theater chase
}

/*
   The star is a pentagram with 5 leds from each strip contributing to a point.
   If the colors are applied to 1 to 5 and 10 to 15 range of each strip, then we cover the points.
   Once east way to do this is to use pixel counter divided by 5 and mod it and check whether that is not 1
   This applies the change only to the points.

   The reverse of this math applies to the center only
*/
void starRainbow(uint8_t wait, bool centerOnly) {
  uint16_t i, j, positionIndicator;

  for (j = 0; j < 256 * RAINBOW_CYCLES; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      // see comment above
      positionIndicator = ((i / 5) % 3);
      if (centerOnly) {
        if (positionIndicator == 1) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        } else {
          strip.setPixelColor(i, 0); //off the others
        }
      } else {
        if (positionIndicator != 1) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        } else {
          strip.setPixelColor(i, 0); //off the others
        }
      }



    }
    strip.show();
    delay(wait);
  }
}

/*
   Same as regular theater chase but the changes are applied to the parts of the strip that covers the
   points only

   Note the use of a second positionIndicator that helps with the overflow
*/
void starRainbowChasing(uint8_t wait, bool centerOnly) {
  uint16_t positionIndicator, plusPositionIndicator;
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < THEATER_CHASE_CYCLES; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        // see comment above
        positionIndicator = ((i / 5) % 3);
        plusPositionIndicator = (((i + q) / 5) % 3);
        if (centerOnly) {
          if (positionIndicator == 1 || plusPositionIndicator ==1) {
            strip.setPixelColor(i + q, Wheel( (i + j) % 255));
          } else {
            strip.setPixelColor(i, 0); //off the others
          }
        } else {
          if (positionIndicator != 1 || plusPositionIndicator !=1) {
           strip.setPixelColor(i + q, Wheel( (i + j) % 255));
          } else {
            strip.setPixelColor(i, 0); //off the others
          }
        }
      }
      strip.show();
      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }

}
// Slightly different, this makes the rainbow equally distributed throughout the
// strip
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * RAINBOW_CYCLES; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < THEATER_CHASE_CYCLES; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
