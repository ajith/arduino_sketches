/*
This sketch contains the well known and widely used NeoPixels styles, adopted to 
multiple strips on different output pins. The requirement, primarily, is to run multiple
strips synchronized with mostly the same pattern.

The original functions have either been taken from the Arduino example sketches and modified 
for the multilple strips usecase or taken from other open source works (which are acked on the comments
in the relevant funtions.

This work itself is released under MIT License

-----------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2018 Ajith Ranabahu ajith.ranabahu@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <Adafruit_NeoPixel.h>

#define STRIP_COUNT 6
// common number of pixels - this is meant to control pixels 
#define PIXELS 14
// on ATMega328 based Arduinos these are all the PWM pins
// Note that if you change this, the STRIP_COUNT has be changed as well
int PIN[] = {3,5,6,9,10,11};

Adafruit_NeoPixel strip[] = { 
    Adafruit_NeoPixel(PIXELS, PIN[0], NEO_GRB + NEO_KHZ800), 
    Adafruit_NeoPixel(PIXELS, PIN[1], NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(PIXELS, PIN[2], NEO_GRB + NEO_KHZ800), 
    Adafruit_NeoPixel(PIXELS, PIN[3], NEO_GRB + NEO_KHZ800), 
    Adafruit_NeoPixel(PIXELS, PIN[4], NEO_GRB + NEO_KHZ800), 
    Adafruit_NeoPixel(PIXELS, PIN[5], NEO_GRB + NEO_KHZ800)
  };

void setup() {
   for (int count=0;count < STRIP_COUNT;count++) {
     strip[count].begin();
     strip[count].show();
   }
}

void loop() {
  uint8_t speed = 40; // This is the global speed factor.

  // The lower the speed, the faster the lights will become.
  // #Color is a static method so it is fine to take it off the first
  // strip
  multiColorWipe(strip[0].Color(255, 0, 0), speed); // Red
  multiColorWipe(strip[0].Color(0, 255, 0), speed); // Green
  multiColorWipe(strip[0].Color(0, 0, 255), speed); // Blue

 
  for(int chase_size = 10; chase_size > 3; chase_size = chase_size -1){
    multiLargeBlockChase(strip[0].Color(0, 255, 0), chase_size, speed);
   }
   multiRainbowTheaterChase(speed);
  
  for(int position = 0; position < 255; position = position + 40){
    knightRider(1, 4 , 2, Wheel(position));
  }

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
  multiLargeBlockChase(c,3, wait);
}

// Chase like behavior with large blocks of colors - basically 
// a version of theater chase but with a cutomizable width
void multiLargeBlockChase(uint32_t c, uint8_t width, uint8_t wait) {
  for (int j=0; j < 10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < width; q++) {
      for (int i=0; i < PIXELS; i = i + width) {
         for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, c);
         }
       }
       showEffect(wait);
     
      for (int i=0; i < PIXELS; i = i + width) {
        for (int count=0;count < STRIP_COUNT;count++) {
           strip[count].setPixelColor(i+q, 0); 
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

// The following function is inspired from the knightrider sketch in 
// https://github.com/technobly/NeoPixel-KnightRider/blob/master/NeoPixel_KnightRider.ino
// It has been modified to use multiple strips in parallel
//
// Cycles - one cycle is scanning through all pixels left then right (or right then left)
// Speed - how fast one cycle is (32 with 16 pixels is default KnightRider speed)
// Width - how wide the trail effect is on the fading out LEDs.  The original display used
//         light bulbs, so they have a persistance when turning off.  This creates a trail.
//         Effective range is 2 - 8, 4 is default for 16 pixels.  Play with this.
// Color - 32-bit packed RGB color value.  All pixels will be this color.
// knightRider(cycles, speed, width, color);
void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color) {
  uint32_t old_val[PIXELS]; // up to 256 lights!
  for(int i = 0; i < cycles; i++){
    //forward
    for (int count = 1; count< PIXELS; count++) {
          for (int scount=0; scount < STRIP_COUNT;scount++) {
           strip[scount].setPixelColor(count, color);                         
           old_val[count] = color;
           for(int x = count; x>0; x--) {
             old_val[x-1] = dimColor(old_val[x-1], width);
             strip[scount].setPixelColor(x-1, old_val[x-1]); 
           }
          }
      showEffect(speed);
    }
    //reverse
    for (int count = PIXELS - 1; count>=0; count--) {
      for (int scount=0; scount < STRIP_COUNT;scount++) {
       strip[scount].setPixelColor(count, color);
       old_val[count] = color;
       for(int x = count; x<= PIXELS ;x++) {
         old_val[x-1] = dimColor(old_val[x-1], width);
         strip[scount].setPixelColor(x+1, old_val[x+1]);
       }
      }
     showEffect(speed);
    }
  }
}

// Dims the color in proportion of the width.
// The dimness is calculated by dividing the the RGB components
// by the width and doing a masking with that color space.
// Example
// red  color&0xFF0000 <- extracts the red component
//     dividing that by width reduces it to a 20th in brightness
//     masking with the original color space FF0000 gives the values for the original component, if anything has overflowed
uint32_t dimColor(uint32_t color, uint8_t width) {
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}
