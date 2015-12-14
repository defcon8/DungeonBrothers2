#ifndef CMENU_H
#define CMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "cmenuitem.h"
#include <vector>


class cMenu
{
	private:
		void init();
		void buildMenu();
		void createSurface();
		void ttfInit();
		void menuActionDispatcher(int id);
		void menuActionExit();
		int selectedmenuitem;
		TTF_Font* ttffont;
	    SDL_Color green;
	    SDL_Color yellow;
	    SDL_Color black;
	    vector<cMenuItem> *currentmenuitems;
	public:
		cMenu();
		~cMenu();
		void render();
		void move(int direction);
		void select();
        vector<cMenuItem> menuitems;
        vector<cMenuItem> menupath;
        SDL_Texture* texture;
		SDL_Surface* surface;
		SDL_Surface* background;
	protected:
};

#endif
