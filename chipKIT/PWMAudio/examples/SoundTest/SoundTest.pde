#include <PWMAudio.h>
#include "gameover.h"

PWMAudio sound(AUDIO, 11025);

void setup()
{
  sound.begin();
  sound.queueSample(gameover, gameover_len, 0, 0);
}

void loop()
{
}
