#include <WProgram.h>
#include <DebouncedInput.h>

DebouncedInput::DebouncedInput(byte pin_, unsigned long dbt_, boolean pullup_ = true)
{
    this->pin = pin_;
    this->debounceTime = dbt_;
    this->pullup = pullup_;
}

void DebouncedInput::begin()
{
    if (this->pullup) {
        pinMode(this->pin, INPUT_PULLUP);
    } else {
        pinMode(this->pin, INPUT);
    }
    this->value = digitalRead(this->pin);
    this->lastValue = this->value;
}

int DebouncedInput::read() 
{
    int r;
    unsigned long now = millis();
    r = digitalRead(this->pin);
    if (r != this->lrt) {
        this->lrt = r;
        this->lastChange = now;
    }
    if (now - this->lastChange > this->debounceTime) {
        if (this->value != this->lrt) {
            this->value = this->lrt;
        }
    }
    return this->value;
}

boolean DebouncedInput::changed()
{
    int r = this->read();
    if (r != this->lastValue) {
        this->lastValue = r;
        return true;
    }
    return false;
}
