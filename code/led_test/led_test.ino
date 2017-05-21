#include <Adafruit_NeoPixel.h>

// led pin
#define PIN 13 // 4 or 13

// number of LEDS
#define NUMPIXELS      22


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int r;
int b;
int g;

int delayval = 50; 

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  r = random(0,150);
  b = random(0,150);
  g = random(0,150);

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r,g,b)); 

    pixels.show(); 

    delay(delayval); 

  }
}
