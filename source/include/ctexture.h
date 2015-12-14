#ifndef CTEXTURE_H
#define CTEXTURE_H

#include <string>
#include <SDL2/SDL.h>

using namespace std;

class cTexture
{
	
	private:
        //Image dimensions
        int width;
        int height;
	
	public:
		cTexture();
		~cTexture();
	
		//The actual hardware texture
		SDL_Texture* handle;
		SDL_Surface* surfacehandle;
	
        //Loads image at specified path
        bool loadFromFile(string path);

        //Deallocates texture
        void free();

        //Gets image dimensions
        int getWidth();
        int getHeight();
		
		
	protected:
};

#endif
