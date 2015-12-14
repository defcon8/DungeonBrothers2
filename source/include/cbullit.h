#ifndef CBULLIT_H
#define CBULLIT_H

#include "../include/clevelobject.h"

using namespace std;

class cBullit : public cLevelObject
{
	private:
		float scalex, scaley;
		float velocityx, velocityy;
		int angle, speed;
	public:
		 cBullit(const char* tilesource, int spriteheight, int spritewidth, int angleref, int speedref);
		~cBullit();
		void aI();
	protected:
};

#endif
