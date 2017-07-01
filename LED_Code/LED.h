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

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

/*
 *Create an array  with 30 strips, using previously defined pins. 
 */
 
Adafruit_NeoPixel strips[]{
  
  Adafruit_NeoPixel(30, PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_10, NEO_GRB + NEO_KHZ800),
  
  Adafruit_NeoPixel(30, PIN_11, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_12, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_13, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_14, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_15, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_16, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_17, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_18, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_19, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_20, NEO_GRB + NEO_KHZ800),
  
  Adafruit_NeoPixel(30, PIN_21, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_22, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_23, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_24, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_25, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_26, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_27, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_28, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_29, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(30, PIN_30, NEO_GRB + NEO_KHZ800),
};

//Predefine Standard colors
uint32_t blank = strips[0].Color(0, 0, 0);
uint32_t blue = strips[0].Color(0, 0, 255);  //101
uint32_t red = strips[0].Color(255, 0, 0);  //102
uint32_t green = strips[0].Color(0, 255, 0);  //103

uint32_t currentColor = blue;
