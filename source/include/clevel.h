#ifndef CLEVEL_H
#define CLEVEL_H

#include <string>
#include <list>
#include "../include/csprite.h"
#include "../include/cspritelayer.h"
#include "../include/cplayer.h"


using namespace std;

class cLevel
{
	public:
		cLevel();
		~cLevel();
		
		bool load(string file);
		bool save(string file);
		
		cSprite* backgroundlayer;
		cSpriteLayer* levellayer;
		cPlayer* playerobject;
		
		list<iLevelObject*> objects;
		list<iLevelBackground*> backgrounds;
		
	protected:
};

#endif
