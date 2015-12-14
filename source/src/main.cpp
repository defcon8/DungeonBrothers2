#include <stdio.h>
#include "../include/cgame.h"
#include "../include/cdebug.h"


int main (int argc, char* argv[])
{
	
  // Get out of static context
  cGame *game = new cGame();
  game->run();
  delete game;
  
  return 0;	
}

