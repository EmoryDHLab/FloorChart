// Library for Arduino-to-Arduino communication (see https://www.arduino.cc/en/Tutorial/MasterWriter)
// Note on Mega: SDA/data pin is 20 and SCL/clock pin is 21
#include <Wire.h>
#define NODE_ADDRESS 2 //unique address of this I2C node
#define PAYLOAD_SIZE 4 // # bytes master I2C node receives

unsigned char nodePayload[PAYLOAD_SIZE]; // store data to send to master
                               // note: though bytes use less memory, we need ints to uniquely address all switches

// Library for enabling membrane switch controller
#include <Keypad.h>

// ROWS cannot exceed 30 and COLS cannot exceed 16; otherwise, edit Keypad.h and .cpp
const unsigned char ROWS = 30;
const unsigned char COLS = 15;



//BOTTOM-INPUT
byte rowPins[ROWS] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, // 1-15
                      22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50}; // 16-30

// TOP-OUTPUT
byte colPins[COLS] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 14, 15, 16}; // 1-15


Keypad keypad = Keypad(rowPins, colPins, ROWS, COLS);


void setup(){
  Serial.begin(9600); //Sets the speed of the serial port - I am not sure this is needed if we have the following line
  Wire.begin(NODE_ADDRESS); // join i2c bus with NODE_ADDRESS
  Wire.onRequest(requestEvent); // function to call when master reads data (on master request)
}

//There is a need to add a buffer in this loop
void loop(){
  keypad.getKeys();
}


void requestEvent(){
  
  unsigned long int buttonChange = keypad.getButtonChange();
  nodePayload[0] = NODE_ADDRESS;
  unsigned char row = (unsigned char)((buttonChange >> 24) & 0xF);
  unsigned char col = (unsigned char)((buttonChange >> 16) & 0xF);
  unsigned char type = (unsigned char)(buttonChange & 0xF);

  nodePayload[1] = row;
  nodePayload[2] = col;
  nodePayload[3] = type;
  Wire.write(nodePayload, PAYLOAD_SIZE); // Wire.write takes (data, size) where size is in bytes
//  Print for debugging purposes
  Serial.println("Node Address:");
  Serial.println(NODE_ADDRESS);
  Serial.println("Row:" );
  Serial.println(nodePayload[1]);
  Serial.println("Col:" );
  Serial.println(nodePayload[2]);
  Serial.println("Type:" );
  Serial.println(nodePayload[3]);
}

