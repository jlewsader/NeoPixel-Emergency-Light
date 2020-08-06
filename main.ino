/*
   Emergency Light Ver 1.4
   J. Lewsader
   8/2020
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define LEDPIN 0          // Change this to whatever pin your NeoPixel Data is using.
#define BUTTONPIN 4       // Change this to whatever pin your switch/button is on. MAKE SURE IT WILL SUPPORT INTERRUPTS!!
#define NUM_PIXELS 60     // Change this to the number of pixels in your strip.

// Some global button variables
int selector = 0;
unsigned long button_time = 0;
unsigned long last_button_time = 0;
boolean cancelLoop = false;

// See the Adafruit NeoPixel documentation for details.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

/* This is the Interrupt code for the momentary button/switch.
** For use on the ESP8266, we have to place it in RAM with ICACHE_RAM_ATTR.  */
ICACHE_RAM_ATTR void increment() {

  button_time = millis();
  //check to see if increment() was called in the last 250 milliseconds. Nice little debounce.
  if (button_time - last_button_time > 250)
  {
    selector++;
    if (selector > 6) selector = 0;
    cancelLoop = true;
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
  /*  For strip.Color(R, G, B), use 0-255 to set intensity.
      The last number is a delay 0-255 range.   */
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
        // Amber Right
        ClearLights();
        ArrowRight(strip.Color(255, 150, 0), 30);
        break;
      }
    case 4  : {
        // Amber Left
        ArrowLeft(strip.Color(255, 150, 0), 30);
        break;
      }
    case 5  : {
        // Amber Center Out
        CenterOut(strip.Color(255, 150, 0), 30);
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

  cancelLoop = false;
}

// Effects Functions Below ************************************************

void ArrowRight(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if (cancelLoop == false) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
    else {
      ClearLights();
      break;
    }
  }
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if (cancelLoop == false) {
      strip.setPixelColor(i, 0);
    }
    else {
      ClearLights();
      break;
    }
  }
  strip.show();
  delay(wait);
}

void ArrowLeft(uint32_t c, uint8_t wait) {
  for (uint16_t i = strip.numPixels(); i > 0 ; i--) {
    if (cancelLoop == false) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
    else {
      ClearLights();
      break;
    }
  }
  for (uint16_t i = strip.numPixels(); i > 0 ; i--) {
    if (cancelLoop == false) {
      strip.setPixelColor(i, 0);
    }
    else {
      ClearLights();
      break;
    }
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
  delay(wait * 4);
}

void CenterOut(uint32_t c, uint8_t wait) {
  uint16_t j = 1;
  uint16_t HALFWAY = (strip.numPixels() / 2) - 1;

  for (uint16_t i = HALFWAY; i < strip.numPixels(); i++) {
    if (cancelLoop == false) {
      strip.setPixelColor(HALFWAY - j, c);    //scans to the right
      strip.setPixelColor(i, c);              //scans to the left
      j++;
      strip.show();
      delay(wait);
      if (j == HALFWAY) {  
        j = 1;
      }
    } else {
      ClearLights();
      break;
    }
  }
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  delay(wait);
}
