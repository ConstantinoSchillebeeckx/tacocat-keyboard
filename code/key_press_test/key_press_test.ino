#include <Keypad.h>

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


byte rowPins[ROWS] = {19, 2, 23, 18, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 33, 25, 26, 27, 14, 32}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
