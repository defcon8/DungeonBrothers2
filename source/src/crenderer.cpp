#include "../include/cdebug.h"
#include "../include/cconfig.h"
#include "../include/cdebug.h"
#include "../include/crenderer.h"
#include "../include/cwindow.h"
#include "../include/cworld.h"

cRenderer::cRenderer()
{
	TRACE("Construct","cRenderer()");
	init();
}

cRenderer::~cRenderer()
{
	TRACE("Deconstruct","cRenderer()");
	SDL_DestroyRenderer(handle);
}

bool cRenderer::init(){
	// Create Renderer
	handle = SDL_CreateRenderer(cWorld::getInstance()->window->handle, -1, 0);
	
	if (handle==NULL){
      TRACE("Init","Could not create renderer: %s\n", SDL_GetError());
      return false;
	}
    
    // Setup logical display size
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");  
	SDL_RenderSetLogicalSize(handle, cWorld::getInstance()->config->displaywidth, cWorld::getInstance()->config->displayheight);
    SDL_SetRenderDrawBlendMode(handle,SDL_BLENDMODE_BLEND);
    
    // Fill background with black
    SDL_SetRenderDrawColor(handle, 0, 0, 0, 255);
	SDL_RenderClear(handle);
	SDL_RenderPresent(handle);
    
    TRACE("Init","Renderer initialized.");
	return true;	
}


