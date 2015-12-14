#ifndef CLEVELOBJECT_H
#define CLEVELOBJECT_H

#include "../include/ilevelobject.h"
#include "../include/cspritelayer.h"

class cLevelObject : public iLevelObject 
{
	
	#define UP 1
	#define RIGHT 2
	#define DOWN 3
	#define LEFT 4
	#define NONE 0
	
	private:
		struct objpos{
        	int x,y;
    	};
    
    	void getHorScanPos(cSpriteLayer* p_object, int column, int &begin, int &end);
    	void getPositionInLevel(cSpriteLayer* sourceobject, objpos* position, int& colstart, int& colend, int& rowstart, int& rowend);
    	bool collideDown(cSpriteLayer* sourceobject, objpos* position, int colstart, int colend, int rowstart, int rowend, int pixels);
		
	public:
		cSpriteLayer* gfxlayer;
    	int x,y;

    	cLevelObject(const char* tilesource, int spriteheight, int spritewidth);
    	virtual ~cLevelObject();
    	virtual void update();
    	virtual void render();
    	virtual bool isAlive();
    	virtual void aI();
    	bool checkDirectionCollision(cSpriteLayer* object, int direction);
    	bool checkDirectionCollision(cSpriteLayer* object, int direction, int pixels);
    	int movespeed;
    	bool isalive;

	protected:
};

#endif
