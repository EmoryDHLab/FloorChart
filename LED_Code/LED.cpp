//
//  LED.cpp
//  
//
//  Created by Noah Sutter on 2/7/18.
//

#include <stdio.h>
#include "LED.h"

LED::LED() {
    for (unsigned char i = 0; i < NUMSTRIPS; i++) {
        strip s {
            Adafruit_NeoPixel(i, pinArray[i], NEO_GRB + NEO_KHZ800),
            0
        };
        strips[i] = s;
    }
}
