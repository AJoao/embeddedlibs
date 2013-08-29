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

#include "Network.h"

void HTTPServer::server()
{
	TCPClient *c;
	char line[1024];
	short r,i;
	char page[1024];
	short eou;
	struct get argv[MAX_GET_OPTIONS];
	int argc;
	char *ptr1, *ptr2;

	c = this->select();
	if(c)
	{
		if(c->available()>0)
		{
			r=c->readln(line,1023);
			while(r>0)
			{
				if(strncmp("GET ",line,4)==0)
				{
					eou = 4;
					while((line[eou]!=' ') && (eou<r))
						eou++;
					memset(page,0,1024);
					strncpy(page,&line[4],eou-4);
					page[eou-4]=0;
				}
				r=c->readln(line,1023);
			}
			if(r<0)
			{
				c->println("ABORT");
				c->flush();
				c->close();
			} else {
				r=0;
				// We have a valid header, and something interesting in page.
				// Now we need to parse the GET paramemeters.

				ptr1 = page;
				while((*ptr1!='?') && (*ptr1!=0))
				{
					ptr1++;
				}
				argc=0;
				if(*ptr1!=0)
				{
					*ptr1 = 0;
					ptr1++;
					argv[argc].key = ptr1;
					argv[argc].val = NULL;
					while(*ptr1!=0)
					{
						if(*ptr1=='=')
						{
							*ptr1=0;
							ptr1++;
							argv[argc].val = ptr1;
						}
						if(*ptr1=='&')
						{
							*ptr1=0;
							ptr1++;
							if(argc<MAX_GET_OPTIONS-1)
							{
								argc++;
							}
							argv[argc].key = ptr1;
							argv[argc].val = NULL;
						}	
						ptr1++;
					}
					argc++;
				}

				for(i=0; i<MAX_WEB_PAGES; i++)
				{
					if(this->pages[i].url!=NULL)
					{
						if(strcmp(page,this->pages[i].url)==0)
						{
							c->println("HTTP/1.1 200 OK");
							c->println("Content-type: text/html");
							c->println("Connection: close");
							c->println("");
							this->pages[i].function(c,argv,argc);
							c->flush();
							c->close();
							r=1;
						}
					}
				}
				if(r==0)
				{
					c->println("HTTP/1.1 404 Not Found");
					c->println("Content-type: text/html");
					c->println("Connection: close");
					c->println("");
					c->println("<h1>Not Found</h1>");
					c->flush();
					c->close();
				}
			}
		}
	}
}

short HTTPServer::addPage(char *url, int (*function)(TCPClient *, struct get *, int))
{
	unsigned short i;
	for(i=0; i<MAX_WEB_PAGES; i++)
	{
		if(this->pages[i].url==NULL)
		{
			this->pages[i].url = strdup(url);
			this->pages[i].function = function;
			return i;
		}
	}
	return -1;
}
