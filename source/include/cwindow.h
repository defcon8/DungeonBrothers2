#ifndef CWINDOW_H
#define CWINDOW_H

#include <SDL2/SDL.h>
#include <map>

class cWindow
{
	public:
		cWindow();
		~cWindow();
		SDL_Window *handle;
		
	private:
		bool init();
	protected:
};

#endif
