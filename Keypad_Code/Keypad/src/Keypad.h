/*
 ||
 || @file Keypad.cpp
 || @version 3.1
 || @author Noah Sutter - based on keypad library: https://playground.arduino.cc/Code/Keypad
 || @contact noahsutterwork@gmail.com
 ||
 || @description
 || | This library provides a simple interface for using matrix
 || | keypads. It supports multiple keypresses while maintaining
 || | backwards compatibility with the old single key library.
 || | It also supports user selectable pins and definable keymaps.
 || #
 ||
 || For use on floor chart project
 ||
 */


#ifndef KEYPAD_H
#define KEYPAD_H

#include <QueueArray.h>

// bperrybap - Thanks for a well reasoned argument and the following macro(s).
// See http://arduino.cc/forum/index.php/topic,142041.msg1069480.html#msg1069480
#ifndef INPUT_PULLUP
#warning "Using  pinMode() INPUT_PULLUP AVR emulation"
#define INPUT_PULLUP 0x2
#define pinMode(_pin, _mode) _mypinMode(_pin, _mode)
#define _mypinMode(_pin, _mode)  \
do {							 \
	if(_mode == INPUT_PULLUP)	 \
		pinMode(_pin, INPUT);	 \
		digitalWrite(_pin, 1);	 \
	if(_mode != INPUT_PULLUP)	 \
		pinMode(_pin, _mode);	 \
}while(0)
#endif


#define OPEN LOW
#define CLOSED HIGH

typedef char KeypadEvent;
typedef unsigned int uint;
typedef unsigned long ulong;

// Made changes according to this post http://arduino.cc/forum/index.php?topic=58337.0
// by Nick Gammon. Thanks for the input Nick. It actually saved 78 bytes for me. :)
typedef struct {
    uint8_t rows;
    uint8_t columns;
} KeypadSize;

#define LIST_MAX 900	// Max number of keys on the active list


//class Keypad : public Key, public HAL_obj {
class Keypad {
public:
    QueueArray<unsigned long int> buttonChangeBuffer; //short should support 65536 buttons or up to a 256 x 256 matrix as well as 65536 send states
    
    
	Keypad(unsigned char *row, unsigned char *col, uint8_t numRows, uint8_t numCols);

	virtual void pin_mode(byte pinNum, byte mode) { pinMode(pinNum, mode); }
	virtual void pin_write(byte pinNum, boolean level) { digitalWrite(pinNum, level); }
	virtual int  pin_read(byte pinNum) { return digitalRead(pinNum); }

	// AV note: uint defined by typedef, here defined as unsigned int / the default (C++ things)
    bool bitMap[LIST_MAX];
    bool keys[LIST_MAX];

    unsigned long int getButtonChange();
    bool getKeys();
    void setDebounceTime(uint);

private:
    unsigned long startTime;
    void scanKeys();
    bool updateList();

    unsigned char *rowPins;
    unsigned char *columnPins;
	KeypadSize sizeKpd;
	uint debounceTime;
};

#endif

