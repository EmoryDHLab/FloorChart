# Keypad LED Control
Code for I2C communication between 3 Arduinos for membrane switch control of LED display

/keypad.LED.master.ino

    - Controls LED display
    - Requests button events from slaves 1 and 2 used to turn on a corresponding LED
    - Keeps track of the state of the grid to determine the new color of the LED

/keypad.slave.1.ino

    - Uses custom version of Keypad library to listen for key presses on membrane switch control pad 
    - Waits for Master to request data, and upon request will send key code and its address (1)

/keypad.slave.2.ino

    - NOTE: though duplicates first slave, uses different pin assignment due to physical constraints and different address for I2C communication
    - Uses custom version of Keypad library to listen for key presses on membrane switch control pad 
    - Waits for Master to request data, and upon request will send key code and its address (2)

/keypad_membrane-switch-control-pad-and-LEDs.fzz
    
    - Fritzing (http://fritzing.org/download/) file for viewing Arduino wiring and schematic

/BLUEFUIT.h

    - For use with keypad.LED.master.ino

/LED.h
    
    - For use with keypad.LED.master.ino

/Keypad/
    
    - For installation on Arduino IDE 


!WARNING!

This system uses a customized version of the Keypad library. Install the version provided in this folder.
