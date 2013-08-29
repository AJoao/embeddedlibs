#include <WProgram.h>
#include <DSPI.h>
#include <Thermocouple.h>

Thermocouple::Thermocouple(DSPI *spibus, unsigned char ss)
{
    this->spi = spibus;
    this->ss_pin = ss;
}

void Thermocouple::begin()
{
    pinMode(this->ss_pin, OUTPUT);
    digitalWrite(this->ss_pin, HIGH);
    this->spi->begin();
}

void Thermocouple::update()
{
    this->spi->setTransferSize(DSPI_32BIT);
    digitalWrite(this->ss_pin, LOW);
    this->data.value = this->spi->transfer(0);
    digitalWrite(this->ss_pin, HIGH);

    this->internal_ = this->data.internal / 2.0;
    this->external_ = this->data.external / 4.0;

    this->flags = this->data.fault ? TC_FAULT : 0;
    this->flags |= this->data.oc ? TC_OC : 0;
    this->flags |= this->data.scg ? TC_SCG : 0;
    this->flags |= this->data.scv ? TC_SCV : 0;
}

float Thermocouple::internal()
{
    return this->internal_;
}

float Thermocouple::external()
{
    return this->external_;
}

unsigned char Thermocouple::status()
{
    return this->flags;
}
