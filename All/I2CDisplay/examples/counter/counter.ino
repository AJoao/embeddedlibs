#include <Wire.h>
#include <I2CDisplay.h>

// Connect SCL of the display to Analog 5 (i2c clock)
// Connect SCA of the display to Analog 4 (i2c data)

I2CDisplay disp;

void setup() {  
  disp.begin(0);
}

void loop() {
  static unsigned char c = 0;
  disp.setValue(c);
  delay(100);
  c++;
  c = c % 100;
}
