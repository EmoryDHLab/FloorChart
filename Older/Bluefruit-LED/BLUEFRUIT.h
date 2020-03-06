/*
 * BLUEFRUIT BLE UART Instance Variables
 */

SoftwareSerial BLUEFRUIT(10, 11); // Board - RX, TX

uint8_t buffer[5]; //RX Buffer for BLUEFRUIT
int buffer_location = 0; //BLUEFRUIT RX Buffer location
