/*
cap8_sensor.cpp
*/

#include "cap8_sensor.h"

cap8_sensor::cap8_sensor(){
}


boolean cap8_sensor::begin(uint8_t addr){
	Wire.begin();

	_addr = addr;

	 readRegister(PRODUCT_ID);
  
  // Useful debugging info
  
  Serial.print("Product ID: 0x");
  Serial.println(readRegister(PRODUCT_ID), HEX);
  Serial.print("Manuf. ID: 0x");
  Serial.println(readRegister(MANUFACTURER_ID), HEX);
  Serial.print("Revision: 0x");
  Serial.println(readRegister(REVISION), HEX);
  

  if ( (readRegister(PRODUCT_ID) != 0x71) ||
       (readRegister(MANUFACTURER_ID) != 0x5D) ||
       (readRegister(REVISION) != 0x01)) {
    return false;
  }
  // allow multiple touches
  writeRegister(MULT_TOUCH_CONF, 0); 
  // speed up a bit
  writeRegister(STANDBY_CONF, 0x30);
  return true;
}


uint8_t  cap8_sensor::touched(void) {
  uint8_t st = readRegister(INPUT_STATUS);
  if (st) {
    writeRegister(MAIN_CONTROL, readRegister(MAIN_CONTROL) & ~MAIN_INT);
  }
  return st;
}

static uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

static void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

uint8_t cap8_sensor::readRegister(uint8_t reg){
	Wire.beginTransmission(_addr);
    i2cwrite(reg);
    Wire.endTransmission();
    Wire.requestFrom(_addr, 1);
    return (i2cread());
}

void cap8_sensor::writeRegister(uint8_t reg, uint8_t value){
	Wire.beginTransmission(_addr);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();


}