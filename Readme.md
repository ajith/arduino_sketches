# Notes on Multi strips
Many literature suggests that you cannot use multiple PWM pins at once due to Arduinos limitations. See [here](https://forum.arduino.cc/index.php?topic=289698.0) for an example.

However, the Arduino libarary page talks about being able to declare multi pin PWM.
See [here](https://learn.adafruit.com/adafruit-neopixel-uberguide?view=all#faq-36) for the Arduino docs that indicate this can be done.

Note that the original sketchs have to be modifed to get this working

Something like

```
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
...
```
Note the use the strip arrays and the actions on each array.

See the multiple_strips_basic sketch for a working example on this.
