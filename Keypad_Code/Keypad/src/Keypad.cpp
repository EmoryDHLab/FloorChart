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
#include <Keypad.h>

// <<constructor>> Allows pin configuration, and keypad sizes.
Keypad::Keypad(unsigned char *row, unsigned char *col, uint8_t numRows, uint8_t numCols) {
    rowPins = row;
	columnPins = col;
	sizeKpd.rows = numRows;
	sizeKpd.columns = numCols;
	setDebounceTime(10);

	startTime = 0;
}


// Returns a button change from the front of the queue
// Returns 0 if no button changed
unsigned long int Keypad::getButtonChange() {
    if (!buttonChangeBuffer.isEmpty()) {
        return buttonChangeBuffer.pop();
    } else {
        return 0;
    }
}

// Populate the key list.
// Scans keys and updates the activity based on that
// Returns whether there was any activity and if the buttons changed
bool Keypad::getKeys() {
	bool keyActivity = false;

	// Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
	if ( (millis()-startTime)>debounceTime ) {
		scanKeys();
		keyActivity = updateList();
		startTime = millis();
	}

	return keyActivity;
}



// Private : Hardware scan
// Gets the current state of the buttons by checking for impdeance
void Keypad::scanKeys() {
	// Re-intialize the row pins. Allows sharing these pins with other hardware.
	for (unsigned char r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT_PULLUP);
	}
	// bitMap stores ALL the keys that are being pressed.
	for (unsigned char c=0; c<sizeKpd.columns; c++) {
		pin_mode(columnPins[c],OUTPUT);
		pin_write(columnPins[c], LOW);	// Begin column pulse output.
		for (unsigned char r=0; r<sizeKpd.rows; r++) {
            bitMap[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = !pin_read(rowPins[r]); // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[c],HIGH);
		pin_mode(columnPins[c],INPUT);
	}
}



// Update the state of the keys based on the bitmap form the last hardware scan.
// Returns true if any keys on the list changed state.
bool Keypad::updateList() {
	bool anyActivity = false;
	// Add the button changes to the button change buffer
    // loop runs through each potential button to check if there is a change
	for (unsigned char r=0; r<sizeKpd.rows; r++) {
		for (unsigned char c=0; c<sizeKpd.columns; c++) {
            // index is the current index of the button we are checking
            unsigned short index = (unsigned short)(((unsigned short)r * (unsigned short)sizeKpd.columns) + c);
            // button is current state of the button we are checking
            bool button = bitMap[index];
            
            // if button is pressed (is now reading low) and was not pressed in the past
            // button was pressed
            if (button && !keys[index]) {
                Serial.print("Button pressed (row: ");
                Serial.print(r+1);
                Serial.print(" col: ");
                Serial.print(c+1);
                Serial.println(")");
                anyActivity = true;
                // Updating state of buttons
                keys[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = button;
                // Adding the changed keys to the buffer
                // adds one to button because 0 is no key - see getButtonChange()
                unsigned long int buttonChange = ((unsigned long int)r<<24) | ((unsigned long int)c<<16) | (button+1);
                buttonChangeBuffer.push(buttonChange);
                
            // if button is not pressed (is reading high) and was not pressed in the past
            // button was released
            } else if (!button && keys[index]) {
                Serial.print("Button released (row: ");
                Serial.print(r+1);
                Serial.print(" col: ");
                Serial.print(c+1);
                Serial.println(")");
                anyActivity = true;
                // Updating the state of buttons
                keys[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = button;
                // Adding the changed keys to the buffer
                // adds one to button because 0 is no key - see getButtonChange()
                unsigned long int buttonChange = ((unsigned long int)r<<24) | ((unsigned long int)c<<16) | (button+1);
                buttonChangeBuffer.push(buttonChange);
            }
		}
	}
	return anyActivity;
}

// Minimum debounceTime is 1 mS. Any lower *will* slow down the loop().
void Keypad::setDebounceTime(uint debounce) {
	debounce<1 ? debounceTime=1 : debounceTime=debounce;
}
