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
 * either expressed or implied, of Majenko Technologies
 ********************************************************************************/

#include "Network.h"

// Set the port into listen mode.
void TCPServer::listen(unsigned short port)
{
	this->localPort = port;
	this->state = TCP_LISTEN;
}

// Return the next connection in the list to handle.  NULL if no connections
// available.
TCPClient *TCPServer::select()
{
	unsigned short i,j;
	for(i=0; i<MAX_TCP_CONNECTIONS; i++)
	{
		j = (this->currentConnection + i + 1) % MAX_TCP_CONNECTIONS;
		if(this->connections[j].state==TCP_ESTABLISHED)
		{
			this->currentConnection = j;
			return &this->connections[j];
		}
	}
	this->currentConnection = 0;
	return NULL;
}

// Add a new client connection
TCPClient *TCPServer::getFreeConnection()
{
	int i;
	for(i=0; i<MAX_TCP_CONNECTIONS; i++)
	{
		if(this->connections[i].state == TCP_CLOSED)
		{
			return &(this->connections[i]); 
		}
	}
	return NULL;
}
