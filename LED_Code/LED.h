/*
 * Define every pin. Pin #0 and #1 on Arduino Mega don't work.
 */

#define PIN_1 40
#define PIN_2 41
#define PIN_3 42
#define PIN_4 43
#define PIN_5 44
#define PIN_6 45
#define PIN_7 46
#define PIN_8 47
#define PIN_9 48
#define PIN_10 49

#define PIN_11 50
#define PIN_12 51
#define PIN_13 52
#define PIN_14 53
#define PIN_15 A15
#define PIN_16 A14
#define PIN_17 A13
#define PIN_18 A12
#define PIN_19 A11
#define PIN_20 A10

#define PIN_21 A9
#define PIN_22 A8
#define PIN_23 A7
#define PIN_24 A6
#define PIN_25 A5
#define PIN_26 A4
#define PIN_27 A3
#define PIN_28 A2
#define PIN_29 A1
#define PIN_30 A0

#define NUMSTRIPS 30

#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needssed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Creating a pixel type that can hold the
typedef struct {
    Adafruit_NeoPixel neopixelStrip;
    unsigned char state;
} strip;

/*
 *Create an array  with 30 strips, using previously defined pins. 
 */



class LED {
public:
    LED();
    strip strips[NUMSTRIPS];
    int pinArray[NUMSTRIPS] = {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 0xA15, 0xA14, 0xA13, 0xA12, 0xA11, 0xA10, 0xA9, 0xA8, 0xA7, 0xA6, 0xA5, 0xA4, 0xA3, 0xA2, 0xA1};
    //Predefine Standard colors
    uint32_t blank = strips[0].neopixelStrip.Color(0, 0, 0);
    uint32_t blue = strips[0].neopixelStrip.Color(0, 0, 255);  //101
    uint32_t red = strips[0].neopixelStrip.Color(255, 0, 0);  //102
    uint32_t green = strips[0].neopixelStrip.Color(0, 255, 0);  //103
    
private:
};



