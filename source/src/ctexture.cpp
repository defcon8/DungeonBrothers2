#include "../include/cdebug.h"
#include "../include/cworld.h"
#include "../include/crenderer.h"
#include "../include/ctexture.h"
#include <SDL2/SDL_image.h>

cTexture::cTexture()
{
	TRACE("Construct","cTexture()");
	//Initialize
    handle = NULL;
    surfacehandle = NULL;
    width = 0;
    height = 0;
}

cTexture::~cTexture()
{
	TRACE("Deconstruct","cTexture()");
	//Deallocate
    free();
}

bool cTexture::loadFromFile(string path){
    
     //The final texture
    SDL_Texture* newtexture = NULL;

    //Load image at specified path
    surfacehandle = IMG_Load(path.c_str());
    if(surfacehandle == NULL){
        TRACE("Texture","Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }else{
        //Color key image
        SDL_SetColorKey(surfacehandle, SDL_TRUE, SDL_MapRGB(surfacehandle->format, 0, 0, 0));
        
         //Create texture from surface pixels
        newtexture = SDL_CreateTextureFromSurface(cWorld::getInstance()->renderer->handle, surfacehandle);
        
		if(newtexture == NULL){
            TRACE("Texture", "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }else{
            //Get image dimensions
            width = surfacehandle->w;
            height = surfacehandle->h;
        }

        // BW: Keep the surface for now, we still need to analyze it for slopes
    }

	TRACE("Texture", "Texture loaded into memory: %s", path.c_str());

    //Return success
    handle = newtexture;
    return handle != NULL;
}

void cTexture::free()
{
    //Free texture if it exists
    if(handle != NULL){
        SDL_DestroyTexture(handle);
        handle = NULL;
        width = 0;
        height = 0;
    }
    
    if(surfacehandle != NULL){
    	//If not yet released by getslopes()
    	SDL_FreeSurface(surfacehandle);
    	surfacehandle = NULL;
	}
}


int cTexture::getWidth()
{
    return width;
}

int cTexture::getHeight()
{
    return height;
}
