#ifndef CWORLD_H
#define CWORLD_H

#include "../include/ilevelobject.h"
#include "../include/ilevelbackground.h"

#define MODE_GAME 0
#define MODE_MENU 1
#define MODE_EDIT 2

class cConfig;
class cCamera;
class cMenu;
class cRenderer;
class cWindow;
class cLevel;
class cFunctions;
class cLevelEditor;

class cWorld
{
	public:
		~cWorld();
		static cWorld* getInstance();
		void init();
		
		int gamemode;
		bool done;
		
		cConfig *config;
		cCamera *cam;
		cMenu *menu;
		cWindow *window;	
		cRenderer *renderer;
		cLevel *level;
		cLevelEditor *leveleditor;
		cFunctions *functions;
		
		void setRenderLevel(bool value);
		bool getRenderLevel();
		
	private:
		cWorld();
		static cWorld* instance;
		
		bool renderlevel;
		
	protected:
};

#endif
