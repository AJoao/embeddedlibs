#include <GLCD.h>

// How many lines to display.
#define MAX_LINES 20

// Arrays to store the line coordinates in
int xa[MAX_LINES];
int ya[MAX_LINES];
int xb[MAX_LINES];
int yb[MAX_LINES];

// These are the directions of each of the points.
int dxa,dya,dxb,dyb;

void setup()
{
  // If you have your GLCD backlight PWM controlled, change this line to use the
  // right PWM channel for your system:
  analogWrite(44,128);
  
  // These are the pins the GLCD is attached to.  Adjust for your setup.
  //         DI, RW, E,  D0, D1, D2, D3, D4, D5, D6, D7, CS1,CS2,RES
  GLCD.begin(30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43);

  // Set up some starting values for the coordinates
  xa[0] = rand()&127;
  xb[0] = rand()&127;
  ya[0] = rand()&63;
  yb[0] = rand()&63;
  
  dxa = 3;
  dya = -2;
  dxb = 1;
  dyb = -1;
}

void loop()
{
  // Clear the screen
  GLCD.cls();
  
  // Draw all the lines in their current positions
  for(int i=0; i<MAX_LINES; i++)
  {
    GLCD.draw(xa[i],ya[i],xb[i],yb[i],1);
  }
  
  // Update the screen (you won't see any changes until you do).
  GLCD.update();

  // Slide the arrays down one entry
  for(int i=MAX_LINES-1; i>0; i--)
  {
    xa[i] = xa[i-1];
    ya[i] = ya[i-1];
    xb[i] = xb[i-1];
    yb[i] = yb[i-1];
  }
  
  // Generate new coordinates.
  xa[0]+=dxa;
  ya[0]+=dya;
  xb[0]+=dxb;
  yb[0]+=dyb;

  // Limit the coordinates and bounce the lines off the edges.
  if(xa[0]<0)
  {
    xa[0] = 0;
    dxa = -dxa;
  }
  if(xa[0]>127)
  {
    xa[0] = 127;
    dxa = -dxa;
  }
  
  if(ya[0]<0)
  {
    ya[0] = 0;
    dya = -dya;
  }
  if(ya[0]>63)
  {
    ya[0] = 63;
    dya = -dya;
  }
  
  if(xb[0]<0)
  {
    xb[0] = 0;
    dxb = -dxb;
  }
  if(xb[0]>127)
  {
    xb[0] = 127;
    dxb = -dxb;
  }
  
  if(yb[0]<0)
  {
    yb[0] = 0;
    dyb = -dyb;
  }
  if(yb[0]>63)
  {
    yb[0] = 63;
    dyb = -dyb;
  }
}
