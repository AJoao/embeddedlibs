#include <GLCD.h>

/* Mandelbrot generator.
 * Controlled by three potentiometers:
 * A0 = left/right
 * A1 = up/down
 * A2 = zoom.
 */

const int
  bits        = 12,   // Fractional resolution
  pixelWidth  = 128,  // LCD dimensions
  pixelHeight = 64,
  iterations  = 128;  // Fractal iteration limit or 'dwell'

void setup()
{
  // If you have your GLCD backlight PWM controlled, change this line to use the
  // right PWM channel for your system:
  analogWrite(44,255);

  // These are the pins the GLCD is attached to.  Adjust for your setup.
  //         DI, RW, E,  D0, D1, D2, D3, D4, D5, D6, D7, CS1,CS2,RES  
  GLCD.begin(30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43);
  GLCD.cls();
  GLCD.update();
}

void loop()
{
  int x,y,n;
  long a,b,a2,b2,posReal,posImag;
  long startReal, startImag, incReal, incImag;
  float rangeReal, rangeImag, centerReal, centerImag;

  float an0 = (analogRead(0)-512) / 512.0;
  float an1 = (analogRead(1)-512) / 512.0;
  float an2 = (analogRead(2)) / 512.0;
  
  centerReal = an0;
  centerImag = an1;
  rangeReal = an2*2.0;
  rangeImag = an2;

  startReal   = (long)((centerReal - rangeReal * 0.5)   * (float)(1 << bits)),
  startImag   = (long)((centerImag + rangeImag * 0.5)   * (float)(1 << bits)),
  incReal     = (long)((rangeReal / (float)pixelWidth)  * (float)(1 << bits)),
  incImag     = (long)((rangeImag / (float)pixelHeight) * (float)(1 << bits));

  GLCD.cls();

  posImag = startImag;
  for(y = 0; y < pixelHeight; y++) {
    posReal = startReal;
    for(x = 0; x < pixelWidth; x++) {
      a = posReal;
      b = posImag;
      for(n = iterations; n > 0 ; n--) {
        a2 = (a * a) >> bits;
        b2 = (b * b) >> bits;
        if((a2 + b2) >= (4 << bits)) break;
        b  = posImag + ((a * b) >> (bits - 1));
        a  = posReal + a2 - b2;
      }
      if(n&1)
        GLCD.setPixel(x,y);
      else
        GLCD.clearPixel(x,y);
      posReal += incReal;
    }
    posImag -= incImag;
  }
  GLCD.update();
}

