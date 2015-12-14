#ifndef CRENDERER_H
#define CRENDERER_H

#include <SDL2/SDL.h>

class cRenderer
{
	public:
		cRenderer();
		~cRenderer();
		SDL_Renderer *handle;
	private:
		bool init();
	protected:
};

#endif
