#include <Adafruit_NeoPixel.h>

#define STRIP_COUNT 2
// common number of pixels - this is meant to control pixels 
#define PIXELS 30

int PIN[] = {5,6};
Adafruit_NeoPixel strip[] = { Adafruit_NeoPixel(PIXELS, PIN[0], NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(PIXELS, PIN[1], NEO_GRB + NEO_KHZ800)};

void setup() {
   for (int count=0;count < STRIP_COUNT;count++) {
     strip[count].begin();
     strip[count].show();
   }
}

void loop() {
  uint8_t speed = 50; // This is the global speed factor.

  // The lower the speed, the faster the lights will become.
  // #Color is a static method so it is fine to take it off the first
  // strip
  multiColorWipe(strip[0].Color(255, 0, 0), speed); // Red
  multiColorWipe(strip[0].Color(0, 255, 0), speed); // Green
  multiColorWipe(strip[0].Color(0, 0, 255), speed); // Blue
  
  multiTheaterChase(strip[0].Color(0, 255, 0), speed); //Green
  multiTheaterChase(strip[0].Color(100, 100, 0), speed); //red + green

  multiRainbowTheaterChase(speed);
  
}

// Fill the dots one after the other with a color
void multiColorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i < PIXELS; i++) {
    for (int count=0;count < STRIP_COUNT;count++) {
     strip[count].setPixelColor(i, c);
   }
   showEffect(wait);
  }
}

// shows effect on all knows strips
void showEffect(uint8_t wait){
   for (int count=0;count < STRIP_COUNT;count++) {
     strip[count].show();
   }
   delay(wait);
}


// Theatre-style crawling lights for multiple strips
void multiTheaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j < 10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < PIXELS; i=i+3) {
         for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, c); //turn every third pixel on
         }
       }
       showEffect(wait);
     
      for (int i=0; i < PIXELS; i=i+3) {
        for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, 0); //turn every third pixel off
         }
      }
    }
  }
}
//
// Theatre-style crawling lights with rainbow effect
// This is pretty much the same thing as the previous one except that
// the color is selected via a method Wheel
void multiRainbowTheaterChase(uint8_t wait) {
  for (int j=0; j < 50; j++) {  //do 50 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < PIXELS; i=i+3) {
         for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, Wheel( (i+j) % 255)); //turn every third pixel on
         }
       }
       showEffect(wait);
     
      for (int i=0; i < PIXELS; i=i+3) {
        for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, 0); //turn every third pixel off
         }
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

