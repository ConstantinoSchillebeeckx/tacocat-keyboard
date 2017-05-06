#include <Keypad.h>
#include <Adafruit_NeoPixel.h>



#define PIN 4
#define NUMPIXELS 22

const byte ROWS = 5; //four rows
const byte COLS = 7; //four columns

// the . character represents keys in the matrix that don't
// exist on the physical keyboard
char hexaKeys[ROWS][COLS] = {
  {'y','x','w','v','u','t','t'},  
  {'f','e','d','c','b','a','.'},
  {'s','r','q','p','o','n','m'}, 
  {'l','k','j','i','h','g','.'},  
  {'7','6','5','4','3','2','1'} 
};

int r;
int b;
int g;

byte rowPins[ROWS] = {19, 2, 23, 18, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 33, 25, 26, 27, 14, 32}; //connect to the column pinouts of the keypad



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    r = random(0,150);
    b = random(0,150);
    g = random(0,150);
    
//    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(2, pixels.Color(r,g,b)); 
      pixels.show(); 
//    }
    Serial.print(customKey);
    Serial.print(' ');
    Serial.print(r);
    Serial.print(' ');
    Serial.print(g);
    Serial.print(' ');
    Serial.println(b);
  }
}
