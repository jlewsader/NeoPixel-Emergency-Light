# NeoPixel Emergency Light

I am a volunteer firefighter. After looking at all of the expensive options for POV lights, I decided I wanted to 
make my own custom emergency light for my vehicle. This gave me the option to have RGB lights for all colors without the HIGH cost!

The light is a single 60 pixel, 1 meter strip of NeoPixels controlled by an Adafruit Feather HUZZAH ESP8266 (All I had available at the time).
The code should be simple enough to run on any microcontroller with very little changes.

Currently I have set 7 flash patterns -
1. Triple Flash - Blue
2. 2-color WigWag - Blue/White
3. Single-color WigWag - Blue
4. Right Traffic Advisor - Amber
5. Left Traffic Advisor - Amber
6. Center-Out Traffic Advisor - Amber
7. WigWag - Amber

I am now currently working on ideas on how to enclose and mount the light in the vehicle and how I want to wire power for it.

## INSTRUCTIONS

### BOM

ID | Part name | Part number | Quantity
-- | --------- | ----------- | --------
R1 | 10k Resistor | 1234-abcd | 10
L1 | Red LED | 2345-asdf | 5
A1 | Arduino | ABX00066 | 1

### Step 1: Assemble the circuit

Assemble the circuit following the diagram `layout.png` attached to the sketch.

IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
and minimize distance between Arduino and first pixel.  Avoid connecting
on a live circuit...if you must, connect GND first.

### Step 2: Load the code

Upload the code contained in this sketch on to your board.

## License
My code - Public Domain  
The libraries and pre-built code - Whatever license they are released under.
