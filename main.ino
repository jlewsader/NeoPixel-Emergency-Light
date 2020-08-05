/*
   Emergency Light Ver 1.3
   J. Lewsader
   8/2020
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define LEDPIN 0
#define BUTTONPIN 2

int selector = 0;
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

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

ICACHE_RAM_ATTR void increment() {
  
  button_time = millis();
  //check to see if increment() was called in the last 250 milliseconds
  if (button_time - last_button_time > 250)
    {
      selector++;
      if (selector > 6) selector = 0;
      last_button_time = button_time;
    }
}

void setup() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), increment, FALLING);
  strip.begin();
  strip.show();
}

void loop () {
  switch (selector) {
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
        // Blue WigWag
        WigWag(strip.Color(0, 0, 255), 75);
        break;
      }
    case 3  : {
        // Arrow Right
        ArrowRight(strip.Color(255, 150, 0), 30); //Amber Right Signal
        break;
      }
    case 4  : {
        // Arrow Left
        ArrowLeft(strip.Color(255, 150, 0), 30); //Amber Left Signal
        break;
      }
    case 5  : {
        // Amber Center Out
        CenterOut(strip.Color(255, 150, 0), 30); //Amber Right Signal
        break;
      }
    case 6  : {
        // Amber WigWag
        WigWag(strip.Color(255, 150, 0), 191);
        break;
      }
    // case 7  : {
    //     // OFF
    //     ClearLights();
    //     break;
    //   }
  }
}

/*
  For strip.Color(R, G, B), use 0-255 to set intensity
  for each color (R)ed, (G)reen, (B)lue
  The last number is a delay 0-255 range.
*/


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

void TripFlash(uint32_t c, uint8_t wait) {
  for (int x = 0; x < 3; x++) {
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

void CenterOut(uint32_t c, uint8_t wait) {
  uint16_t j = 1;
  uint16_t HALFWAY = (strip.numPixels() / 2) - 1;

  for (uint16_t i = HALFWAY; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(HALFWAY - j, c);    //scans to the right
    strip.setPixelColor(i, c);              //scans to the left
    j++;
    strip.show();
    delay(wait);
    if (j == 29) {
      j = 1;
    }
  }
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  delay(wait);
}
