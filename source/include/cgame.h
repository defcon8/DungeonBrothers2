#ifndef CGAME_H
#define CGAME_H

#include <SDL2/SDL.h>

class cSprite;

using namespace std;

class cGame
{
	public:
		cGame();
		~cGame();
		void run();
	private:
		bool done;
		bool init();
		void dispose();
		void eventLoop();
		void render();
		void renderMenuMode();
		void renderGameMode();
		void renderEditMode();
		void renderMenu();
		void renderObjects();
		void renderBackgrounds();
		void updateObjects();
		void update();
		void commonEvents(SDL_Event *event);
		void gameModeEvents(SDL_Event *event);
		void menuModeEvents(SDL_Event *event);
		
	protected:
};

#endif
