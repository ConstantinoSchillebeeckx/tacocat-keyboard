#include <Adafruit_NeoPixel.h>

// led pin
#define PIN 13 // 4 (R) or 13 (L)

// number of LEDS
#define NUMPIXELS      22


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int r;
int b;
int g;

int delayval = 50; 
int counter = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
}

void loop() {

  r = random(0,150);
  b = random(0,150);
  g = random(0,150);

  /*
 * Colors that flicker (r,g,b)
 * 14,14,14
 * 124,141,142
 * flickering only occurs when no LiPo battery hooked up
 */
 

  Serial.print(r);
  Serial.print(' ');
  Serial.print(g);
  Serial.print(' ');
  Serial.println(b);

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r,g,b)); 

    pixels.show(); 

    delay(delayval); 

  }

  delay(20);

  counter++;
}
