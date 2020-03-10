/*
   To turn on the specific pixels, open the Serial monitor.
   Then input the row number 1-30. Press Enter.
   Then Input the column number 1-30. Press Enter.
   This will turn on// turn off the selected pixel.
   If the user inputs 100 and 100, all pixels will turn on for 5 sec. This can be used to reset
   all the pixels or to check if all the pixels work.
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
   WIRE: define
*/
#define PAYLOAD_SIZE 4 // how many bytes to expect from each I2C salve node
#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes
#define NODE_READ_DELAY 100 // Some delay between I2C node reads

// The size of the copper grid
#define ROWSIZE 30 // also refers to the number of pixels in each strip
#define COLSIZE 30 // also refers to the number of strips

#define NUMSTATES 7 // sets the number of states, used to loop through the number of states

// Led strips pin assignments
unsigned char stripsToPins[NUMSTRIPS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, // 1-15
                                         //30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}; // 16-30

                                         uint32_t startTime;

                                         // holds the state of each pixel and the last time it was changed
                                         // the most significant 24 bits are the time (supports continuous software running for 194 days)
                                         // the least significant 8 bits are the state (supports 256 states)
                                         uint32_t pixelStates[COLSIZE * ROWSIZE];

                                         // holds the neopixel strips
                                         Adafruit_NeoPixel strips[COLSIZE];

                                         bool first = true;

                                         /*
                                            LED: notes
                                         */
                                         // IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
                                         // pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
                                         // and minimize distance between Arduino and first pixel.  Avoid connecting
                                         // on a live circuit...if you must, connect GND first.

                                         // Setup  initializes the LED grid and sets up the i2c bus
void setup() {

  Wire.begin(); // join i2c bus

  Serial.begin(9600);
  Serial.print("Start");

  // Not sure if this is necessary
  while (!Serial); // Waits for Serial to begin

  // Initializing the strips
  for (int i = 0; i < COLSIZE; i++) {
    // Parameter 1 = number of pixels in strip
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    //Adafruit_NeoPixel strip = Adafruit_NeoPixel(ROWSIZE, stripsToPins[i], NEO_GRB + NEO_KHZ800);
    strips[i].updateLength(ROWSIZE);
    strips[i].setPin(stripsToPins[i]);
    strips[i].updateType(NEO_GRB + NEO_KHZ800);


    strips[i].begin();
    strips[i].setBrightness(60); // Brightness goes from 0 (off) to 255(max brightness).
    strips[i].show(); // Initializes all pixels to off

    for (int j = 0; j < ROWSIZE; j++) {
      // Initialize the pixel states to 0
      pixelStates[i * ROWSIZE + j] = 0;

      // Make all pixels blank
      strips[i].setPixelColor(j, strips[i].Color(0, 0, 0));
    }
    strips[i].show();
  }
  startTime = millis();
}


void loop()
{
  // This needs to be done in loop not just setup for some reason, not sure why TODO
  if (first) {
    for (int i = 0; i < COLSIZE; i++) {
      strips[i].begin();
    }
    first = false;
  }
  // Repeatedly flash red and blue pixels for all pixels specified previously
  for (int i = 0; i < COLSIZE; i++) {
    for (int j = 0; j < ROWSIZE; j++) {

      // Make all pixels red
      strips[i].setPixelColor(j, strips[i].Color(100, 0, 0));
      strips[i].show();
      delay(50);
    }
    Serial.println("Set one strip RED");
    strips[i].show();
  }
  delay(1000);
  for (int i = 0; i < COLSIZE; i++) {
    for (int j = 0; j < ROWSIZE; j++) {

      // Make all pixels blue
      strips[i].setPixelColor(j, strips[i].Color(0, 0, 100));
      strips[i].show();
      delay(50);
    }
    strips[i].show();
    Serial.println("Set one strip BLUE");
  }
  delay(1000);
  // waits a reasonable amount
  delay(NODE_READ_DELAY);
}
