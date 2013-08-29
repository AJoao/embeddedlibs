/********************************************************************************
 * Copyright (c) 2013, Majenko Technologies and S.J.Hoeksma
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Majenko Technologies.
 ********************************************************************************/

#include <Wire.h>
#include "I2CDisplay.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

static inline void wiresend(unsigned char x) {
#if ARDUINO >= 100
  Wire.write((unsigned char)x);
#else
  Wire.send(x);
#endif
}

static inline unsigned char wirerecv(void) {
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}

const unsigned char digits[] = {
  0b01111110,
  0b00010010,
  0b10111100,
  0b10110110,
  0b11010010,
  0b11100110,
  0b11101110,
  0b00110010,
  0b11111110,
  0b11110110,
};

////////////////////////////////////////////////////////////////////////////////

void I2CDisplay::begin(unsigned char addr) {
  unsigned char i;

  if (addr > 7) {
    addr = 7;
  }
  i2caddr = addr;

  Wire.begin();

  
  // set defaults!
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(MCP23017_IODIRA);
  wiresend(0xFF);  // all inputs on port A
  Wire.endTransmission();

  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(MCP23017_IODIRB);
  wiresend(0xFF);  // all inputs on port B
  Wire.endTransmission();

  for (i=0; i<16; i++) {
    this->pinMode(i, OUTPUT);
    this->digitalWrite(i, LOW);
  }
  this->leftDecimal = LOW;
  this->rightDecimal = LOW;
}

void I2CDisplay::begin(void) {
  begin(0);
}

void I2CDisplay::pinMode(unsigned char p, unsigned char d) {
  unsigned char iodir;
  unsigned char iodiraddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8)
    iodiraddr = MCP23017_IODIRA;
  else {
    iodiraddr = MCP23017_IODIRB;
    p -= 8;
  }

  // read the current IODIR
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(iodiraddr);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
  iodir = wirerecv();

  // set the pin and direction
  if (d == INPUT) {
    iodir |= 1 << p; 
  } else {
    iodir &= ~(1 << p);
  }

  // write the new IODIR
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(iodiraddr);
  wiresend(iodir);	
  Wire.endTransmission();
}

unsigned short I2CDisplay::readGPIOAB() {
  unsigned short ba = 0;
  unsigned char a;

  // read the current GPIO output latches
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(MCP23017_GPIOA);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 2);
  a = wirerecv();
  ba = wirerecv();
  ba <<= 8;
  ba |= a;

  return ba;
}

void I2CDisplay::writeGPIOAB(unsigned short ba) {
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(MCP23017_GPIOA);	
  wiresend(ba & 0xFF);
  wiresend(ba >> 8);
  Wire.endTransmission();
}

void I2CDisplay::digitalWrite(unsigned char p, uint8_t d) {
  unsigned char gpio;
  unsigned char gpioaddr, olataddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8) {
    olataddr = MCP23017_OLATA;
    gpioaddr = MCP23017_GPIOA;
  } else {
    olataddr = MCP23017_OLATB;
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  // read the current GPIO output latches
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(olataddr);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
   gpio = wirerecv();

  // set the pin and direction
  if (d == HIGH) {
    gpio |= 1 << p; 
  } else {
    gpio &= ~(1 << p);
  }

  // write the new GPIO
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(gpioaddr);
  wiresend(gpio);	
  Wire.endTransmission();
}

void I2CDisplay::pullUp(unsigned char p, uint8_t d) {
  unsigned char gppu;
  unsigned char gppuaddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8)
    gppuaddr = MCP23017_GPPUA;
  else {
    gppuaddr = MCP23017_GPPUB;
    p -= 8;
  }


  // read the current pullup resistor set
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(gppuaddr);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
  gppu = wirerecv();

  // set the pin and direction
  if (d == HIGH) {
    gppu |= 1 << p; 
  } else {
    gppu &= ~(1 << p);
  }

  // write the new GPIO
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(gppuaddr);
  wiresend(gppu);	
  Wire.endTransmission();
}

unsigned char I2CDisplay::digitalRead(uint8_t p) {
  unsigned char gpioaddr;

  // only 16 bits!
  if (p > 15)
    return 0;

  if (p < 8)
    gpioaddr = MCP23017_GPIOA;
  else {
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  // read the current GPIO
  Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
  wiresend(gpioaddr);	
  Wire.endTransmission();
  
  Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
  return (wirerecv() >> p) & 0x1;
}

void I2CDisplay::setLeftDigit(unsigned char d) {
  unsigned char i;
  d = d % 10;
  d = digits[d];
  d |= this->leftDecimal ? 1 : 0;
  for (i=0; i<8; i++) {
    this->digitalWrite(i, d & (1 << i) ? HIGH : LOW);
  }
}

void I2CDisplay::setRightDigit(unsigned char d) {
  unsigned char i;
  d = d % 10;
  d = digits[d];
  d |= this->rightDecimal ? 1 : 0;
  for (i=0; i<8; i++) {
    this->digitalWrite(8 + i, d & (1 << i) ? HIGH : LOW);
  }
}

void I2CDisplay::setValue(unsigned char d) {
    this->setLeftDigit(d / 10);
    this->setRightDigit(d % 10);
}

void I2CDisplay::setLeftDecimal(unsigned char d) {
    this->digitalWrite(0, d);
    this->leftDecimal = d;
}

void I2CDisplay::setRightDecimal(unsigned char d) {
    this->digitalWrite(8, d);
    this->rightDecimal = d;
}
