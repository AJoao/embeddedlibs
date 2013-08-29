#include <GLCD.h>

// Phrases to print

const __attribute__((space(prog))) char *phrases[] = {
  "Hello World",
  "This is GLCD text",
  "scrolling nicely up",
  "the screen.",
  "",
  "",
  "O Romeo, Romeo!",
  "wherefore art thou",
  "Romeo? Deny thy",
  "father and refuse",
  "thy name! Or, if",
  "thou wilt not, be",
  "but sworn my love,",
  "and I'll no longer",
  "be a Capulet.",
  "",
  "'Tis but thy name",
  "that is my enemy.",
  "Thou art thyself,",
  "though not a",
  "Montague.  What's",
  "Montague? It is nor",
  "hand, nor foot, nor",
  "arm, nor face, nor",
  "any other part",
  "belonging to a man.",
  "O, be some other",
  "name! What's in a",
  "name? That which we",
  "call a rose by any",
  "other name would",
  "smell as sweet. So",
  "Romeo would, were",
  "he not Romeo call'd,",
  "retain that dear",
  "perfection which he",
  "owes without that",
  "title. Romeo, doff",
  "thy name; and for",
  "that name, which is",
  "no part of thee,",
  "take all myself.",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  0
};

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
  GLCD.update();
}

void loop()
{
  static int line = 0;
  
  // Set the cursor to center the text on line 8 - yes, that is the 9th line of an 8
  // line screen.  The "screen" is slightly larger so we can print off the bottom
  // especially for nice scrolling.
  GLCD.setGCursor(63-((strlen(phrases[line])*6)/2),64);

  // Print the line
  GLCD.gText(phrases[line]);
  
  // Slowly scroll the screen up 12 lines (it makes for nicer line spacing)
  for(int i=0; i<12; i++)
  {
    // Scroll up 1 pixel
    GLCD.scrollUp();
    // Update the display (you won't see any changes until you do).
    GLCD.update();
    // It's not readable if we go at full speed.  A 120ms delay per pixel line is good enough.
    // try commenting out this line and see what I mean.
    delay(120);
  }
  
  // Move on to the next line of text
  line++;
  
  // If we have reached the end, then go back to the beginning again.
  if(phrases[line]==0)
    line=0;  
}
