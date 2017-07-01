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

#include "LED.h"

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

  /*
   * Setup all the strips and Serial. 
   */
  Serial.begin(9600);
  for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
  {
    
    strips[i].begin();
    strips[i].setBrightness(60); // Brightness goes from 0 (off) to 255(max brightness).
    strips[i].show();
    
  }
}

void loop() {

  
  int loopUserPrompt1 = 1;
  int loopUserPrompt2 = 1;
  int row = 0;
  int column = 0;
  


  /*
   * These loops prompt the user for two numbers from 1 to 30. First the row(the strip to turn on), then the column. 
   * If user types 100 and then 100 all pixels will light up for 5 seconds.
   */
   
  Serial.println("Input Color or Row: (101 - blue, 102 - red, 103 - green, 100 will turn on all pixels)");
  while(loopUserPrompt1 == 1){
    
    if (Serial.available() > 0) 
    {
      int input = Serial.parseInt(); 
      
      if(input == 101)
      {
        
        currentColor = blue;
      
      }
      else if(input == 102)
      {
        
        currentColor = red;
        
      }
      else if(input == 103)
      {
        
        currentColor = green;
        
      }
      else if(input == 100 || input > 0 && input < 31)
      {
        
        row = input-1;
        Serial.println("Input Column: (100 will turn all pixels)");
        
        while(loopUserPrompt2 == 1)
        {
          
          if (Serial.available() > 0) 
          {
            
            int input2 = Serial.parseInt();
            
            if(input2 == 100 || input2 > 0 && input2 < 31)
            {
              
              column = input2-1;
              loopUserPrompt1 = 0;
              loopUserPrompt2 = 0;
              
            }
           }
         }
       }
     }
  }
  stripControl(row, column, currentColor);
}

/*
 * If the user types in two 100s then turn on all the pixels then turn them all off in 5 seconds.
 */
void stripControl(int row, int column, int color)
{
  if(row == 99 && column == 99)
  {
    
    for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++){
      for(int j = 0;j<30;j++) //------------30 pixels per strip
      {
        strips[i].setPixelColor(j, currentColor);
      }
      
      strips[i].show();
      
    }
    
    delay(5000);
    
    for(int i = 0; i < sizeof(strips) / sizeof(*strips); i++)
    {
      
      for(int j = 0;j<30;j++) //------------30 pixels per strip
      {                      
        strips[i].setPixelColor(j, 0, 0, 0);
      }
      
      strips[i].show();
      
    }

      
  }
  /*
   *If the picked pixel is off, turn it on. If the pixel is on, then turn it off.
   */
  else{
    
    if(strips[row].getPixelColor(column) == blank)
    {
      
      strips[row].setPixelColor(column, currentColor);
      strips[row].show();
      
    }
    else
    {
      
      strips[row].setPixelColor(column, 0, 0, 0);
      strips[row].show();
    
    } 
  }
}
