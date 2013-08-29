#ifndef _DEBOUNCEDINPUT_H
#define _DEBOUNCEDINPUT_H

#include <WProgram.h>

class DebouncedInput
{
  private:
    byte pin;
    int value;
    unsigned long lastChange;
    unsigned long debounceTime;
    int lrt;
    boolean pullup;
    int lastValue;
    
  public:
    DebouncedInput(byte pin_, unsigned long dbt_, boolean pullup_);
    void begin();
    int read();
    boolean changed();
};

#endif
