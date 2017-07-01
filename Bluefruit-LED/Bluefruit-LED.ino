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
#include "BLUEFRUIT.h"

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  BLUEFRUIT.begin(9600); //Begin BLUEFRUIT RX/TX

  /*
   * Setup all the strips and Serial. 
   */
  Serial.begin(9600);
  while(!Serial); // Waits for Serial to begin
  
  for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
  {
    strips[i].begin();
    strips[i].setBrightness(60); // Brightness goes from 0 (off) to 255(max brightness).
    strips[i].show();
  }
}

void loop() 
{

  bool colorPicked = false;
  bool needRow = true;
  bool needCol = true;
  int row = 0;
  int column = 0;

  while(BLUEFRUIT.available() > 0) // Clear any unnecessary data sent to BLUEFRUIT.
  {
    
    BLUEFRUIT.read();
    
  }

  Serial.println("Please select a color on your device.");
  
  while (!colorPicked)
  {
    
    if (BLUEFRUIT.available() > 0)
    {
      
      byte recv = BLUEFRUIT.read();
      
      if (recv  == '!')
      {
        
        buffer_location = 0;
        
      }
  
      buffer[buffer_location] = recv;
      buffer_location++;
  
      if (buffer_location > sizeof(buffer) - 1)
      {
        
        if (buffer[1] == 'C')
        {
          
          parseColor();
          colorPicked = true;
          
        }
      }
    }
  }

  /*
   * These loops prompt the user for two numbers from 1 to 30. First the row(the strip to turn on), then the column. 
   * If user types 100 and then 100 all pixels will light up for 5 seconds.
   */
   
  Serial.println("Input Row: (100 will turn on all pixels)");
  
  while(needRow)
  {
    
    if (Serial.available() > 0) 
    {
      
      int input = Serial.parseInt(); 
      
      if(input == 100 || input > 0 && input < 31)
      {
        
        row = input-1;
        Serial.println("Input Column: (100 will turn all pixels)");
        
        while(needCol)
        {
          
          if (Serial.available() > 0) 
          {
            
            int input2 = Serial.parseInt();
             
            if(input2 == 100 || input2 > 0 && input2 < 31)
            {
              column = input2-1;
              needRow = false;
              needCol = false;
            }
          }
        }
      }
    }
  }
  
  stripControl(row, column, currentColor);
  
}

void stripControl(int row, int column, int color)
{
  /*
   * If the user types in two 100s then turn on all the pixels then turn them all off in 5 seconds.
   */
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
      strips[row].show();
      
    } 
  }
}

/**
 * Parses color input from BlueFruit
 */
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
