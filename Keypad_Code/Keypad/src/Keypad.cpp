/*
||
|| @file Keypad.cpp
|| @version 3.1
|| @author Mark Stanley, Alexander Brevig
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for using matrix
|| | keypads. It supports multiple keypresses while maintaining
|| | backwards compatibility with the old single key library.
|| | It also supports user selectable pins and definable keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#include <Keypad.h>

// <<constructor>> Allows custom keymap, pin configuration, and keypad sizes.
Keypad::Keypad(unsigned char *row, unsigned char *col, uint8_t numRows, uint8_t numCols) {
    rowPins = row;
	columnPins = col;
	sizeKpd.rows = numRows;
	sizeKpd.columns = numCols;
	setDebounceTime(10);

	startTime = 0;
}



unsigned long int Keypad::getButtonChange() {
    if (!buttonChangeBuffer.isEmpty()) {
        return buttonChangeBuffer.pop();
    } else {
        return 0;
    }
}

// Populate the key list.
bool Keypad::getKeys() {
    //Serial.println("Get KEYS");
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
void Keypad::scanKeys() {
    //Serial.println("Scan Keys");
	// Re-intialize the row pins. Allows sharing these pins with other hardware.
	for (unsigned char r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT_PULLUP);
	}
	// bitMap stores ALL the keys that are being pressed.
	for (unsigned char c=0; c<sizeKpd.columns; c++) {
		pin_mode(columnPins[c],OUTPUT);
		pin_write(columnPins[c], LOW);	// Begin column pulse output.
		for (unsigned char r=0; r<sizeKpd.rows; r++) {
            bitMap[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = !pin_read(rowPins[r]);
			//bitWrite(bitMap[r], c, !pin_read(rowPins[r]));  // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[c],HIGH);
		pin_mode(columnPins[c],INPUT);
	}
}



// Manage the list without rearranging the keys. Returns true if any keys on the list changed state.
bool Keypad::updateList() {
    //Serial.println("Update List");
	bool anyActivity = false;
	// Add the button changes to the button change buffer
	for (unsigned char r=0; r<sizeKpd.rows; r++) {
		for (unsigned char c=0; c<sizeKpd.columns; c++) {
            bool button = bitMap[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c];
            unsigned short index = (unsigned short)(((unsigned short)r * (unsigned short)sizeKpd.columns) + c);
            if (button && !keys[index]) {
                Serial.print("Button pressed (row: ");
                Serial.print(r+1);
                Serial.print(" col: ");
                Serial.print(c+1);
                Serial.println(")");
                anyActivity = true;
                keys[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = button;
                unsigned long int buttonChange = ((unsigned long int)r<<24) | ((unsigned long int)c<<16) | button;
                buttonChangeBuffer.push(buttonChange);
                
            } else if (!button && keys[index]) {
                Serial.print("Button released (row: ");
                Serial.print(r+1);
                Serial.print(" col: ");
                Serial.print(c+1);
                Serial.println(")");
                anyActivity = true;
                keys[(unsigned short)((unsigned short)r * (unsigned short)sizeKpd.columns) + c] = button;
                
                unsigned long int buttonChange = ((unsigned long int)r<<24) | ((unsigned long int)c<<16) | button;
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



/*
|| @changelog
|| | 3.1 2013-01-15 - Mark Stanley     : Fixed missing RELEASED & IDLE status when using a single key.
|| | 3.0 2012-07-12 - Mark Stanley     : Made library multi-keypress by default. (Backwards compatible)
|| | 3.0 2012-07-12 - Mark Stanley     : Modified pin functions to support Keypad_I2C
|| | 3.0 2012-07-12 - Stanley & Young  : Removed static variables. Fix for multiple keypad objects.
|| | 3.0 2012-07-12 - Mark Stanley     : Fixed bug that caused shorted pins when pressing multiple keys.
|| | 2.0 2011-12-29 - Mark Stanley     : Added waitForKey().
|| | 2.0 2011-12-23 - Mark Stanley     : Added the public function keyStateChanged().
|| | 2.0 2011-12-23 - Mark Stanley     : Added the private function scanKeys().
|| | 2.0 2011-12-23 - Mark Stanley     : Moved the Finite State Machine into the function getKeyState().
|| | 2.0 2011-12-23 - Mark Stanley     : Removed the member variable lastUdate. Not needed after rewrite.
|| | 1.8 2011-11-21 - Mark Stanley     : Added decision logic to compile WProgram.h or Arduino.h
|| | 1.8 2009-07-08 - Alexander Brevig : No longer uses arrays
|| | 1.7 2009-06-18 - Alexander Brevig : Every time a state changes the keypadEventListener will trigger, if set.
|| | 1.7 2009-06-18 - Alexander Brevig : Added setDebounceTime. setHoldTime specifies the amount of
|| |                                          microseconds before a HOLD state triggers
|| | 1.7 2009-06-18 - Alexander Brevig : Added transitionTo
|| | 1.6 2009-06-15 - Alexander Brevig : Added getState() and state variable
|| | 1.5 2009-05-19 - Alexander Brevig : Added setHoldTime()
|| | 1.4 2009-05-15 - Alexander Brevig : Added addEventListener
|| | 1.3 2009-05-12 - Alexander Brevig : Added lastUdate, in order to do simple debouncing
|| | 1.2 2009-05-09 - Alexander Brevig : Changed getKey()
|| | 1.1 2009-04-28 - Alexander Brevig : Modified API, and made variables private
|| | 1.0 2007-XX-XX - Mark Stanley : Initial Release
|| #
*/
