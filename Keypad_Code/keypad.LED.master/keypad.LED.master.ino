/*
 * To turn on the specific pixels, open the Serial monitor.
 * Then input the row number 1-30. Press Enter. 
 * Then Input the column number 1-30. Press Enter.
 * This will turn on// turn off the selected pixel.
 * If the user inputs 100 and 100, all pixels will turn on for 5 sec. This can be used to reset
 * all the pixels or to check if all the pixels work.
 */
 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <SoftwareSerial.h>
#include "LED.h"
//#include "BLUEFRUIT.h"
// Library for Arduino-to-Arduino communication (see https://www.arduino.cc/en/Tutorial/MasterWriter)
// Note on Mega: SDA/data pin is 20 and SCL/clock pin is 21
#include <Wire.h>
// Library for enabling membrane switch controller
#include <Keypad.h>

/*
 * WIRE: define 
 */
#define PAYLOAD_SIZE 4 // how many bytes to expect from each I2C salve node
#define NODE_MAX 2 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes
#define NODE_READ_DELAY 100 // Some delay between I2C node reads

// The size of the copper grid
#define ROWSIZE 30
#define COLSIZE 30


/*
 * LED: notes
 */
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  /*
  * LED: Begin BLUEFRUIT RX/TX (communication)
  */
  //BLUEFRUIT.begin(9600); 

  /*
   * WIRE: Set up communication to keypad.slave Arduino
   */
  Wire.begin(); // join i2c bus 
  
  /*
  * Arduino: begin Serial communication 
  */
  Serial.begin(9600);

   /*
   * LED: Setup all the strips.
   */
   
   /* LEDCODE
  while(!Serial); // Waits for Serial to begin
  for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
  {
    strips[i].begin();
    strips[i].setBrightness(60); // Brightness goes from 0 (off) to 255(max brightness).
    strips[i].show();
  }*/
}


void loop() 
{
  /*
   * LED: Set up currentColor 
   */
   /* LEDCODE
//  bool colorPicked = false;
//  while(BLUEFRUIT.available() > 0) // Clear any unnecessary data sent to BLUEFRUIT.
//  {
//    BLUEFRUIT.read();
//  }
//  Serial.println("Please select a color on your device.");
//  while (!colorPicked)
//  {
//    if (BLUEFRUIT.available() > 0)
//    {
//      byte recv = BLUEFRUIT.read();
//      if (recv  == '!')
//      {
//        buffer_location = 0;
//      }
//      buffer[buffer_location] = recv;
//      buffer_location++;
//      if (buffer_location > sizeof(buffer) - 1)
//      {
//        if (buffer[1] == 'C')
//        {
//          Serial.println("Parsing color selection.");
//          parseColor();
//          colorPicked = true;
//        }
//      }
//    }
//  }
  */
  
  /*
   * WIRE: loop through slave 1 and 2 for key transmissions
   *        pass input to stripControl to turn on corresponding LED
   */

   /* LEDCODE
  int ledRow = 999;
  int ledCol = 999;
  */
  
  for (unsigned char nodeAddress = START_NODE; nodeAddress <= NODE_MAX; nodeAddress++) { // we are starting from Node address 1
    Wire.requestFrom(nodeAddress, PAYLOAD_SIZE);    // request data from node#
    
    if(Wire.available() == PAYLOAD_SIZE) {  // if data size is avaliable from nodes
      //for (int i = 0; i < PAYLOAD_SIZE; i++) nodePayload[i] = Wire.read();  // get nodes data
      unsigned char addr = Wire.read();
      unsigned char row = Wire.read();
      unsigned char col = Wire.read();
      unsigned char type = Wire.read();
      if (!(row == 0 && col == 0 && type ==0)) {
        //int nodeAddress = nodePayload[0];
        //int keyCode = nodePayload[1]; // NOTE: Key code range is 0-450, multiply for 451 - 900
        Serial.println("Payload received.");
        Serial.print("Received data from node ");
        Serial.println(addr);
        Serial.print("Row: ");
        Serial.println(row);
        Serial.print("Col: ");
        Serial.println(col);
        Serial.print("Type: ");
        Serial.println(type);
        //ledRow = nodePayload[1]/15 + nodePayload[1]%15;
        //ledCol = nodePayload[1]/30 + nodePayload[1]%30;
        //if (nodeAddress == 2){
        //  ledCol += 15; // we split control over 2 Arduinos, so if it comes from #2 we need to shift the column
        //}
        //stripControl(ledRow, ledCol, blue);
        Serial.println("*************************");
      }      
      }
    }
    delay(NODE_READ_DELAY);
}


/*
 * LED: Turn on LED at input row, column, color
 * If user inputs 100 for both row and column, all LEDs turn on 
 */
 /* LEDCODE
void stripControl(int row, int column, int color)
{
  /*
   * If the user types in two 100s then turn on all the pixels then turn them all off in 5 seconds.
   */
    /* LEDCODE
  if(row == 99 && column == 99)
  {
    for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
    {
      for(int j = 0; j < 30;j++) //------------30 pixels per strip
      {                      
        strips[i].setPixelColor(j, currentColor);
      }
      strips[i].show();
    }
    delay(5000);
    for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
    {
      for(int j = 0; j < 30; j++) //------------30 pixels per strip
      {                      
        strips[i].setPixelColor(j, 0, 0, 0);
      }
      strips[i].show();
    }
  /*
   *If the picked pixel in off, turn it on. If the pixel is on, then turn it off.
   */
    /* LEDCODE
  }
  else
  {
    if(strips[row].getPixelColor(column) == blank)
    {
      strips[row].setPixelColor(column, currentColor);
      strips[row].show();
    }
    else
    {
      strips[row].setPixelColor(column, currentColor);
      //strips[row].setPixelColor(column, 0, 0, 0);
      strips[row].show();
    } 
  }
}
*/

/**
 * LED: Parses color input from BlueFruit, i.e. from Serial
 */
 /* LEDCODE
void parseColor()
{
  Serial.write("Color Selected: \n");
  uint8_t red_value;
  uint8_t green_value;
  uint8_t blue_value;
  red_value = buffer[2];
  green_value = buffer[3];
  blue_value = buffer[4];
  Serial.print("Red Value: ");
  Serial.print(red_value, DEC);
  Serial.print('\n');
  Serial.print("Green Value: ");
  Serial.print(green_value, DEC);
  Serial.print('\n');
  Serial.print("Blue Value: ");
  Serial.print(blue_value, DEC);
  Serial.print('\n');
  currentColor = strips[0].Color(red_value, green_value, blue_value);
}
*?


/*
 * WIRE: function activated by key press
 */
 /*
void receiveEvent(int i) {
  while (Wire.available()) {
    int key = Wire.read();
    Serial.println(key);
    int row = key/15 + key%15;
    int col = key/30 + key%30;
    Serial.println(row);
    Serial.println(col);
  }
}
*/


