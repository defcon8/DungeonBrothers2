#ifndef CPLAYER_H
#define CPLAYER_H

#include "../include/clevelobject.h"

class cPlayer : public cLevelObject
{
	private:
		int gravity, velocityy, velocityx, velocityfall, jumprange, jumpstep, lastdirection;
		void moveByUserInput();
		void jumpPhysics();
		void gravityPhysics();
		void setSprite();
		
	public:
		bool moveup, moveright, movedown, moveleft;
		int facedirection;
		bool isjumping;
		void aI();
		void moveDirection(int direction, bool enabled);
		void jump();
		void fire();
		cPlayer(char* tilesource, int spriteheight, int spritewidth);
		~cPlayer();
	protected:
};

#endif
