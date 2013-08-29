/********************************************************************************
 * Copyright (c) 2012, Majenko Technologies
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
 * either expressed or implied, of the FreeBSD Project.
 ********************************************************************************/

#include <plib.h>
#include <WProgram.h>
#include "Network.h"

DSPI0   spi1;
DSPI1   spi2;
#ifdef _BOARD_MEGA_
DSPI2   spi3;
DSPI3   spi4;
#endif

unsigned char ENC28J60::spiTransfer(unsigned char val)
{
	switch(this->spi)
	{
		case 1:
			return spi1.transfer(val);
		case 2:
			return spi2.transfer(val);
#ifdef _BOARD_MEGA_
		case 3:
			return spi3.transfer(val);
		case 4:
			return spi4.transfer(val);
#endif
	}
}

void ENC28J60::spiBegin()
{
	switch(this->spi)
	{
		case 1:
			return spi1.begin();
		case 2:
			return spi2.begin();
#ifdef _BOARD_MEGA_
		case 3:
			return spi3.begin();
		case 4:
			return spi4.begin();
#endif
	}
}

ENC28J60::ENC28J60(unsigned char chan, unsigned short p)
{
	this->spi = chan;
	this->cspin = p;
	this->spiBegin();
	pinMode(this->cspin,OUTPUT);
	digitalWrite(this->cspin,HIGH);
}

unsigned char ENC28J60::readOp(unsigned char op, unsigned char address)
{
	unsigned char res;
	digitalWrite(this->cspin,LOW);
	this->spiTransfer(op | (address & ADDR_MASK));
	res = this->spiTransfer(0x00);
	if(address & 0x80)
		res = this->spiTransfer(0x00);
	digitalWrite(this->cspin,HIGH);
	return res;
}


void ENC28J60::writeOp(unsigned char op, unsigned char address, unsigned char data)
{
	digitalWrite(this->cspin,LOW);
	this->spiTransfer(op | (address & ADDR_MASK));
	this->spiTransfer(data);
	digitalWrite(this->cspin,HIGH);
}

void ENC28J60::readBuffer(unsigned short len, unsigned char* data)
{
	digitalWrite(this->cspin,LOW);
	this->spiTransfer(ENC28J60_READ_BUF_MEM);
	while (len--)
		*data++ = this->spiTransfer(0x00);
	digitalWrite(this->cspin,HIGH);
	*data='\0';
}

unsigned short ENC28J60::readBufferWord() 
{
	unsigned short result;
	this->readBuffer(2, (unsigned char*) &result);
	return result;
}


void ENC28J60::writeBuffer(unsigned short len, unsigned char* data)
{
	digitalWrite(this->cspin,LOW);
	this->spiTransfer(ENC28J60_WRITE_BUF_MEM);
	while(len--)
		this->spiTransfer(*data++);
	digitalWrite(this->cspin,HIGH);
}

void ENC28J60::setBank(unsigned char address)
{
	if ((address & BANK_MASK) != this->bank) 
	{
		this->writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1|ECON1_BSEL0);
		this->bank = address & BANK_MASK;
		this->writeOp(ENC28J60_BIT_FIELD_SET, ECON1, this->bank>>5);
	}
}

unsigned char ENC28J60::read(unsigned char address)
{
	this->setBank(address);
	return this->readOp(ENC28J60_READ_CTRL_REG, address);
}

void ENC28J60::writeWord(unsigned char address, unsigned short data) 
{
	this->write(address, data & 0xff);
	this->write(address + 1, data >> 8);
}

unsigned short ENC28J60::phyReadH(unsigned char address)
{
	// Set the right address and start the register read operation
	this->write(MIREGADR, address);
	this->write(MICMD, MICMD_MIIRD);
	delayMicroseconds(15);

	// wait until the PHY read completes
	while(this->read(MISTAT) & MISTAT_BUSY);

	// reset reading bit
	this->write(MICMD, 0x00);

	return (this->read(MIRDH));
}

void ENC28J60::write(unsigned char address, unsigned char data)
{
	this->setBank(address);
	this->writeOp(ENC28J60_WRITE_CTRL_REG, address, data);
}


void ENC28J60::phyWrite(unsigned char address, unsigned short data)
{
	this->write(MIREGADR, address);
	this->write(MIWRL, data);
	this->write(MIWRH, data>>8);
	while(this->read(MISTAT) & MISTAT_BUSY)
                delayMicroseconds(15);
}

void ENC28J60::phyWriteWord(unsigned char address, unsigned short data) 
{
	this->write(MIREGADR, address);
	this->writeWord(MIWRL, data);
	while (this->read(MISTAT) & MISTAT_BUSY)
                delayMicroseconds(15);
}

void ENC28J60::clkout(unsigned char clk)
{
        this->write(ECOCON, clk & 0x7);
}

void ENC28J60::init(unsigned char m1, unsigned char m2, unsigned char m3, unsigned char m4, unsigned char m5, unsigned char m6)
{
	this->writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	delay(50);
	this->nextPacketPtr = RXSTART_INIT;
	this->writeWord(ERXSTL, RXSTART_INIT);
	this->writeWord(ERXRDPTL, RXSTART_INIT);
	this->writeWord(ERXNDL, RXSTOP_INIT);
	this->writeWord(ETXSTL, TXSTART_INIT);
	this->writeWord(ETXNDL, TXSTOP_INIT);
	this->write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	this->writeWord(EPMM0, 0x303f);
	this->writeWord(EPMCSL, 0xf7f9);
	this->write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	this->write(MACON2, 0x00);
	this->writeOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
	this->writeWord(MAIPGL, 0x0C12);
	this->write(MABBIPG, 0x12);
	this->writeWord(MAMXFLL, MAX_FRAMELEN);
	this->write(MAADR5, m1);
	this->write(MAADR4, m2);
	this->write(MAADR3, m3);
	this->write(MAADR2, m4);
	this->write(MAADR1, m5);
	this->write(MAADR0, m6);
	this->phyWrite(PHCON2, PHCON2_HDLDIS);
	this->setBank(ECON1);
	this->writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	this->writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
	this->mac[0] = m1;
	this->mac[1] = m2;
	this->mac[2] = m3;
	this->mac[3] = m4;
	this->mac[4] = m5;
	this->mac[5] = m6;
}

unsigned char ENC28J60::getrev(void)
{
	return(this->read(EREVID));
}

unsigned char ENC28J60::linkup(void)
{
	return(this->phyReadH(PHSTAT2) && 4);
}

void ENC28J60::packetSend(unsigned short len, unsigned char* packet)
{
        while (this->readOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
        {
                if( (this->read(EIR) & EIR_TXERIF) ) {
                        this->writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
                        this->writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
                }
        }

        this->writeWord(EWRPTL, TXSTART_INIT);
        this->writeWord(ETXNDL, (TXSTART_INIT+len));
        this->writeOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
        this->writeBuffer(len, packet);
        this->writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

unsigned char ENC28J60::hasRxPkt(void)
{
       return this->read(EPKTCNT) > 0;
}

unsigned short ENC28J60::id()
{
	return this->packetID++;
}

unsigned short ENC28J60::packetReceive(unsigned short maxlen, unsigned char* packet)
{
	unsigned short rxstat;
        unsigned short len;
        if( this->read(EPKTCNT) ==0 ){
                return(0);
        }

        this->writeWord(ERDPTL, this->nextPacketPtr);
        this->nextPacketPtr  = this->readBufferWord();
        len = this->readBufferWord() - 4;
        rxstat  = this->readBufferWord();
        if (len>maxlen-1){
                len=maxlen-1;
        }
        if ((rxstat & 0x80)==0){
                len=0;
        }else{
                this->readBuffer(len, packet);
        }
        this->writeWord(ERXRDPTL, this->nextPacketPtr );
        if ((this->nextPacketPtr - 1 < RXSTART_INIT)
                || (this->nextPacketPtr -1 > RXSTOP_INIT)) {
                this->writeWord(ERXRDPTL, RXSTOP_INIT);
        } else {
                this->writeWord(ERXRDPTL, (this->nextPacketPtr-1));
        }
        this->writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
        return(len);
}

void ENC28J60::setIP(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3)
{
	this->ip = (i0<<24) | (i1<<16) | (i2<<8) | (i3);
}

void ENC28J60::setNetmask(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3)
{
	this->netmask = (i0<<24) | (i1<<16) | (i2<<8) | (i3);
}

void ENC28J60::queueTX(unsigned char *data, unsigned short len)
{
	// Wait for queue space
	//while(this->tx_tail == this->tx_head-1);
	//while(this->tx_head == 0 && this->tx_tail == ETH_BUF_SIZE-1);

	this->tx[this->tx_head].len = len;
	memcpy((void *)&this->tx[this->tx_head].data,(void *)data,len);
	this->tx_head++;
	if(this->tx_head == ETH_BUF_SIZE)
		this->tx_head=0;
}

