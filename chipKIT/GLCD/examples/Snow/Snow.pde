#include <GLCD.h>

// How many dots to display per frame.
#define MAX_DOTS 2000

void setup()
{
  // If you have your GLCD backlight PWM controlled, change this line to use the
  // right PWM channel for your system:
  analogWrite(44,128);
  
  // These are the pins the GLCD is attached to.  Adjust for your setup.
  //         DI, RW, E,  D0, D1, D2, D3, D4, D5, D6, D7, CS1,CS2,RES
  GLCD.begin(30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43);

  // Clear the screen
  GLCD.cls();
}

void loop()
{
  
  // Draw all the dots, and clear just as many
  for(int i=0; i<MAX_DOTS; i++)
  {
    GLCD.setPixel(rand()&127,rand()&63);
    GLCD.clearPixel(rand()&127,rand()&63);
  }
  
  // Update the screen (you won't see any changes until you do).
  GLCD.update();
}
