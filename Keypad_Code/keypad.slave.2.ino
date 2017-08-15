// Library for Arduino-to-Arduino communication (see https://www.arduino.cc/en/Tutorial/MasterWriter)
// Note on Mega: SDA/data pin is 20 and SCL/clock pin is 21
#include <Wire.h>
#define NODE_ADDRESS 2 //unique address of this I2C node
#define PAYLOAD_SIZE 2 // # bytes master I2C node receives
byte nodePayload[PAYLOAD_SIZE]; // store data to send to master

// Library for enabling membrane switch controller
#include <Keypad.h>

// ROWS cannot exceed 30 and COLS cannot exceed 16; otherwise, edit Keypad.h and .cpp
const byte ROWS = 30;
const byte COLS = 15;

// NOTE: the following char array is currently NOT being used to control the pad, but can be used if needed.
// Keys MUST be char; otherwise, edit Keypad.h and .cpp for all defs & functions that use key
// 30x15
char keys[ROWS][COLS] = {  
{ 'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p' }, 
{ 'c',  'e',  'g',  'i',  'k',  'm',  'o',  'q',  's',  'u',  'w',  'y',  'A',  'C',  'E' }, 
{ 'd',  'g',  'j',  'm',  'p',  's',  'v',  'y',  'B',  'E',  'H',  'K',  'N',  'Q',  'T' }, 
{ 'e',  'i',  'm',  'q',  'u',  'y',  'C',  'G',  'K',  'O',  'S',  'W',  '0',  '4',  '8' }, 
{ 'f',  'k',  'p',  'u',  'z',  'E',  'J',  'O',  'T',  'Y',  '3',  '8',  'd',  'i',  'n' },
 
{ 'g',  'm',  's',  'y',  'E',  'K',  'Q',  'W',  '2',  '8',  'e',  'k',  'q',  'w',  'C' }, 
{ 'h',  'o',  'v',  'C',  'J',  'Q',  'X',  '4',  'b',  'i',  'p',  'w',  'D',  'K',  'R' }, 
{ 'i',  'q',  'y',  'G',  'O',  'W',  '4',  'c',  'k',  's',  'A',  'I',  'Q',  'Y',  '6' }, 
{ 'j',  's',  'B',  'K',  'T',  '2',  'b',  'k',  't',  'C',  'L',  'U',  '3',  'c',  'l' }, 
{ 'k',  'u',  'E',  'O',  'Y',  '8',  'i',  's',  'C',  'M',  'W',  '6',  'g',  'q',  'A' },
 
{ 'l',  'w',  'H',  'S',  '3',  'e',  'p',  'A',  'L',  'W',  '7',  'i',  't',  'E',  'P' }, 
{ 'm',  'y',  'K',  'W',  '8',  'k',  'w',  'I',  'U',  '6',  'i',  'u',  'G',  'S',  '4' }, 
{ 'n',  'A',  'N',  '0',  'd',  'q',  'D',  'Q',  '3',  'g',  't',  'G',  'T',  '6',  'j' }, 
{ 'o',  'C',  'Q',  '4',  'i',  'w',  'K',  'Y',  'c',  'q',  'E',  'S',  '6',  'k',  'y' }, 
{ 'p',  'E',  'T',  '8',  'n',  'C',  'R',  '6',  'l',  'A',  'P',  '4',  'j',  'y',  'N' },
 
{ 'q',  'G',  'W',  'c',  's',  'I',  'Y',  'e',  'u',  'K',  '0',  'g',  'w',  'M',  '2' }, 
{ 'r',  'I',  'Z',  'g',  'x',  'O',  '5',  'm',  'D',  'U',  'b',  's',  'J',  '0',  'h' }, 
{ 's',  'K',  '2',  'k',  'C',  'U',  'c',  'u',  'M',  '4',  'm',  'E',  'W',  'e',  'w' }, 
{ 't',  'M',  '5',  'o',  'H',  '0',  'j',  'C',  'V',  'e',  'x',  'Q',  '9',  's',  'L' }, 
{ 'u',  'O',  '8',  's',  'M',  '6',  'q',  'K',  '4',  'o',  'I',  '2',  'm',  'G',  '0' },
 
{ 'v',  'Q',  'b',  'w',  'R',  'c',  'x',  'S',  'd',  'y',  'T',  'e',  'z',  'U',  'f' }, 
{ 'w',  'S',  'e',  'A',  'W',  'i',  'E',  '0',  'm',  'I',  '4',  'q',  'M',  '8',  'u' }, 
{ 'x',  'U',  'h',  'E',  '1',  'o',  'L',  '8',  'v',  'S',  'f',  'C',  'Z',  'm',  'J' }, 
{ 'y',  'W',  'k',  'I',  '6',  'u',  'S',  'g',  'E',  '2',  'q',  'O',  'c',  'A',  'Y' }, 
{ 'z',  'Y',  'n',  'M',  'b',  'A',  'Z',  'o',  'N',  'c',  'B',  '0',  'p',  'O',  'd' },
 
{ 'A',  '0',  'q',  'Q',  'g',  'G',  '6',  'w',  'W',  'm',  'M',  'c',  'C',  '2',  's' }, 
{ 'B',  '2',  't',  'U',  'l',  'M',  'd',  'E',  '5',  'w',  'X',  'o',  'P',  'g',  'H' }, 
{ 'C',  '4',  'w',  'Y',  'q',  'S',  'k',  'M',  'e',  'G',  '8',  'A',  '2',  'u',  'W' }, 
{ 'D',  '6',  'z',  '2',  'v',  'Y',  'r',  'U',  'n',  'Q',  'j',  'M',  'f',  'I',  'b' }, 
{ 'E',  '8',  'C',  '6',  'A',  '4',  'y',  '2',  'w',  '0',  'u',  'Y',  's',  'W',  'q' }
};


//BOTTOM-INPUT
byte rowPins[ROWS] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, // 1-15
                      22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50}; // 16-30

// TOP-OUTPUT
byte colPins[COLS] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 14, 15, 16}; // 1-15


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  Wire.begin(NODE_ADDRESS); // join i2c bus with NODE_ADDRESS
  Wire.onRequest(requestEvent); // activate on master request
}

void loop(){
  nodePayload[0] = NODE_ADDRESS;
  int keyCode = keypad.getKeyCode();
  if (keyCode != NO_KEY){
    nodePayload[1] = keyCode; //  Store keycode, only send on request
    Serial.println("Key pressed: ");
    Serial.println(keyCode);
    Serial.println(nodePayload[1]);
  }
}

void requestEvent(){
  Wire.beginTransmission();
  Wire.write(nodePayload,PAYLOAD_SIZE); // includes address and data
  Wire.endTransmission();

//  Print for debugging purposes
  Serial.println("Node Address:");
  Serial.println(NODE_ADDRESS);
  Serial.println("Key pressed:" );
  Serial.println(nodePayload[1]);
}

