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
#define ROWSIZE 30 // also refers to the number of strips
#define COLSIZE 30 // also refers to the number of pixels in each strip

#define NUMSTATES 7 // sets the number of states, used to loop through the number of states

// Led strips pin assignments
unsigned char stripsToPins[NUMSTRIPS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, // 1-15
                      30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}; // 16-30

uint32_t startTime;

// holds the state of each pixel and the last time it was changed
// the most significant 24 bits are the time (supports continuous software running for 194 days)
// the least significant 8 bits are the state
uint32_t pixelStates[ROWSIZE*COLSIZE];

// holds the neopixel strips
Adafruit_NeoPixel strips[ROWSIZE];


/*
 * LED: notes
 */
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Wire.begin(); // join i2c bus 
  
  Serial.begin(9600);
  // Not sure if this in necessary
  while(!Serial); // Waits for Serial to begin

  // Initializing the strips
  for(int i = 0; i < ROWSIZE; i++) {
    // Parameter 1 = number of pixels in strip
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(COLSIZE, stripsToPins[i], NEO_GRB + NEO_KHZ800);
    strips[i] = strip;
    strips[i].begin();
    strips[i].setBrightness(60); // Brightness goes from 0 (off) to 255(max brightness).
    strips[i].show(); // Initializes all pixels to off

    // Initialize the pixel states to state 0
    for (int j = 0; j < COLSIZE; j++) {
      pixelStates[i*COLSIZE + j] = 0;
    }
  }

  startTime = millis();
}


void loop() 
{
  // goes through each of the slaves asking for button events
  for (unsigned char nodeAddress = START_NODE; nodeAddress <= NODE_MAX; nodeAddress++) { // we are starting from Node address 1
    Wire.requestFrom(nodeAddress, PAYLOAD_SIZE);    // request data from node
    
    if (Wire.available() == PAYLOAD_SIZE) {  // if data is avaliable from node
      // read in the data
      unsigned char addr = Wire.read();
      unsigned char row = Wire.read();
      unsigned char col = Wire.read();
      unsigned char type = Wire.read();

      if (!(row == 0 && col == 0 && type ==0)) { // if there is actual data
        Serial.println("Payload received.");
        Serial.print("Received data from node ");
        Serial.println(addr);
        Serial.print("Row: ");
        Serial.println(row);
        Serial.print("Col: ");
        Serial.println(col);
        Serial.print("Type: ");
        Serial.println(type);
        Serial.println("*************************");
        updatePixel(addr, row, col, type);
        
      }
    }
  }
  // waits a reasonable amount
  delay(NODE_READ_DELAY);
}

// Returns the state of a pixel given the row and the column of the pixel
uint8_t getPixelState(uint8_t row, uint8_t col) {
  return pixelStates[(uint32_t)row * COLSIZE + (uint32_t)col] & 0xF;
}

// Sets the state fo a pixel given the row, column, and new state
void setPixelState(uint8_t row, uint8_t col, uint8_t state) {
  uint32_t index = ((uint32_t)row * COLSIZE + (uint32_t)col);
  pixelStates[index] = (pixelStates[index] & 0xFFF0) & state;
}

// Returns the time of a pixel given the row and the column of the pixel
uint32_t getPixelTime(uint8_t row, uint8_t col) {
  return (uint32_t)pixelStates[(uint32_t)row * COLSIZE + (uint32_t)col]>>8 & 0xFFF;
}

// Sets the time fo a pixel given the row, column, and new time
void setPixelTime(uint8_t row, uint8_t col, uint32_t t) {
  uint32_t index = ((uint32_t)row * COLSIZE + (uint32_t)col);
  pixelStates[index] = (pixelStates[index] & 0x000F) & (t<<8);
}

// Sets the color of a pixel 
void setPixelColor(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue) {
  strips[row].setPixelColor(col, red, green, blue); //sets the pixel color
  strips[row].show(); //updates the color
}

// Updates the pixel, including state, color, and time pressed based on the info received from slave arduinos
void updatePixel(uint8_t addr, uint8_t row, uint8_t col, uint8_t type) {
  // updating the col since the slaves only have half of the pixels each
  col += ((addr - 1) * (COLSIZE / 2));
  if (type == 1) { // pressed
    if (getPixelTime(row, col) - startTime > 200) { // checks that it has been at least 200 milliseconds since the key was last released (combats loose connections)

      // updates the color of the pixel based on the state
      uint8_t state = getPixelState(row, col);
      if (state == 0) {
        setPixelColor(row, col, 100, 100, 100);
      } else if (state == 1) {
        setPixelColor(row, col, 100, 0, 0);
      } else if (state == 2) {
        setPixelColor(row, col, 0, 100, 0);
      } else if (state == 3) {
        setPixelColor(row, col, 0, 0, 100);
      } else if (state == 4) {
        setPixelColor(row, col, 100, 100, 0);
      } else if (state == 5) {
        setPixelColor(row, col, 100, 0, 100);
      } else if (state == 6) {
        setPixelColor(row, col, 0, 100, 100);
      }
      
      //updates the state
      setPixelState(row, col, (state + 1) % NUMSTATES);
    }
  } else if (type == 2) { // released
    // update the time state of the pixel
    setPixelTime(row, col, millis() - startTime);
  }
}


