#include "../include/cdebug.h"
#include "../include/cconfig.h"
#include "../include/cwindow.h"
#include "../include/cworld.h"


cWindow::cWindow()
{
	TRACE("Construct","cWindow()");
	init();
}

cWindow::~cWindow()
{
	TRACE("Deconstruct","cWorld()");
	SDL_DestroyWindow(handle);
}

bool cWindow::init(){
  //initialise SDL2
  SDL_Init(SDL_INIT_VIDEO);
  
  //create window
  if(cWorld::getInstance()->config->fullscreen){
	  TRACE("Init","Enter fullscreen mode.");
  	  handle = SDL_CreateWindow("Dungeon Brothers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }else{
  	  	TRACE("Init","Enter windowed mode.");
		handle = SDL_CreateWindow("Dungeon Brothers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cWorld::getInstance()->config->displaywidth, cWorld::getInstance()->config->displayheight, SDL_WINDOW_OPENGL);
  }

  if (handle==NULL){
  	TRACE("Init","Could not create window: %s\n", SDL_GetError());
	return false;
  }
  
  TRACE("Init","Window initialized.");
  return true;	
}
