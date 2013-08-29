#include <DSPI.h>
#include <Network.h>

ENC28J60 card(1,10);

HTTPServer http;

void setup()
{
  short rv;
  int cardfd;
  int portfd;

  analogWrite(44,128);

  card.init(1,2,3,4,5,6);
  card.setIP(192,168,0,1);
  card.setNetmask(255,255,255,0);  
  cardfd = Network.addInterface(&card);
  Network.addPort(&http);  
  http.addPage("/",root);
  http.addPage("/index.html",root);
  http.listen(80);
}

void loop()
{
  http.server();
}

int root(TCPClient *c, struct get *argv, int argc)
{
  int i;
  c->println("<h1>Analogue Inputs</h1>");
  for(i=0; i<6; i++)
  {
    c->print("Input A");
    c->print(i);
    c->print(": ");
    c->print(analogRead(i));
    c->println("<br/>");
  }
  c->println("<br/>");
  c->println("<h1>Options:</h1>");
  for(i=0; i<argc; i++)
  {
    if(!strcmp("brightness",argv[i].key))
    {
      analogWrite(44,atoi(argv[i].val));
    }
    c->print(argv[i].key);
    c->print(" = ");
    if(argv[i].val != NULL)
      c->print(argv[i].val);
    c->println("<br/>");
  }
  return 200;
}
