/*
 * Emergency Light Ver 1.
 * J. Lewsader
 * 8/2020
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define LEDPIN 0

//Effect toggle switch (Momentary)
const int btn = 2; 
int selector = 0;
boolean isPressed = false; 

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LEDPIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  pinMode(btn, INPUT_PULLUP);
  strip.begin();
  strip.show();
}

void loop ()
{
   
  if (digitalRead(btn) == LOW && isPressed == false ) //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    isPressed = true;  //set to true, so this code will not run again until button released

    selector++; // this is done after the runEffect(), so case 0 will be executed on the first button press 
    if (selector > 6) {
      selector = 0;
    }
  } else if (digitalRead(btn) == HIGH)
  {
    isPressed = false; //button is released, variable reset
  }
        runEffect();
}

 /*
 * For strip.Color(R, G, B), use 0-255 to set intensity
 * for each color (R)ed, (G)reen, (B)lue
 * The last number is a delay 0-255 range.
 */
   
void runEffect(){
  switch(selector) {
    case 0  : { 
                // Triple Flash - Blue
                TripFlash(strip.Color(0, 0, 255), 60);
                break;
              }
    case 1  : {
                // 2-Color WigWag
                WigWag2(strip.Color(0, 0, 255), strip.Color(255, 255, 255), 75); // Blue and White
                break;
              }
    case 2  : {
                // Arrow Left
                ArrowLeft(strip.Color(255, 150, 0), 15); //Amber Left Signal
                break;
              }
    case 3  : {
                // Arrow Right
                ArrowRight(strip.Color(255, 150, 0), 15); //Amber Right Signal
                break;
              }
    case 4  : {
                // Amber WigWag
                WigWag(strip.Color(255, 150, 0), 191); 
                break;
              }
    case 5  : {
                // Blue WigWag
                WigWag(strip.Color(0, 0, 255), 75);
                break;
              }
    case 6  : {
                // OFF
                ClearLights();
                break;
              }
  }
}

// Effects Functions Below ************************************************

void ArrowRight(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(wait);
}

void ArrowLeft(uint32_t c, uint8_t wait) {
    for (uint16_t i = strip.numPixels(); i > 0 ; i--) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
    for (uint16_t i = strip.numPixels(); i > 0 ; i--) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(wait);
}

void WigWag(uint32_t c, uint8_t wait) {
    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, c);
    }
    for (int i = (strip.numPixels() / 2); i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(wait);

    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, 0);
    }
    for (int i = (strip.numPixels() / 2); i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);
}


void WigWag2(uint32_t c, uint32_t c2, uint8_t wait) {
    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, c);
    }
    for (int i = (strip.numPixels() / 2); i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(wait);

    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, 0);
    }
    for (int i = (strip.numPixels() / 2); i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, c2);
    }
    strip.show();
    delay(wait);
}

void ClearLights() {
  for (int i = 0; i < strip.numPixels(); i = i + 1) {
    strip.setPixelColor(i, 0);    //turn every pixel off
  }
  strip.show();
}

void AllOn(uint32_t c) {
  for (int i = 0; i < strip.numPixels(); i = i + 1) {
    strip.setPixelColor(i, c);   
  }
  strip.show();
}

// WIP: Triple Flash
void TripFlash(uint32_t c, uint8_t wait) {
  for(int x = 0; x < 3; x++) {
    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);

    for (int i = 0; i < strip.numPixels(); i = i + 1) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(wait);
  }
  delay(150);
}

/* WIP: CENTER-OUT TRAFFIC ADVISOR **********
void BlinkOuter(uint32_t c, uint8_t wait) {
    strip.setPixelColor(strip.numPixels() - 1, c);
    strip.setPixelColor(0, c);
    strip.show();
    delay(wait);
    strip.setPixelColor(strip.numPixels() - 1, 0);
    strip.setPixelColor(0, 0);
    strip.show();
    delay(wait);
}
*/
