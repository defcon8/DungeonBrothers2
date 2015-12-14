#include "..\include\cconfig.h"
#include "..\include\cdebug.h"
#include "..\include\ccamera.h"
#include "..\include\cbullit.h"
#include "..\include\cworld.h"
#include "..\include\clevel.h"

#include <cmath>

cBullit::cBullit(const char* tilesource, int spriteheight, int spritewidth, int _angle, int _speed) : cLevelObject(tilesource, spriteheight, spritewidth)
{
	TRACE("Construct","cBullit()");
    angle = _angle;
    speed = _speed;

    x = cWorld::getInstance()->level->playerobject->x;
    y = cWorld::getInstance()->level->playerobject->y + (cWorld::getInstance()->level->playerobject->gfxlayer->source->spriteheight / 2);

    //Setup Layer
    gfxlayer = new cSpriteLayer(1,1,spriteheight,spritewidth,false,false,true,0,0,0);

    //Setup Source
    gfxlayer->source->spritespacer = 0;
    gfxlayer->source->setColorKey(0,0,0);
    gfxlayer->source->load(tilesource);
    gfxlayer->source->spritewidthoffset = 0;
    gfxlayer->source->spriteheightoffset = 0;
    gfxlayer->source->spriteheight = spriteheight;
    gfxlayer->source->spritewidth = spritewidth;

    //choose player sprite
    gfxlayer->leveldata[0][0].type=SPRITE;
    gfxlayer->leveldata[0][0].row=0;
    gfxlayer->leveldata[0][0].index=0;
}

cBullit::~cBullit()
{
	TRACE("Deconstruct","cBullit()");
	delete(gfxlayer);
}

void cBullit::aI()
{
    //Movement
    velocityx=(cos(angle*(3.14/180))*speed);
    velocityy=(sin(angle*(3.14/180))*speed);

    x+= velocityx;
    y+= velocityy;
    TRACE("Bullit","Object destroyed CamX: %d", cWorld::getInstance()->config->displaywidth - cWorld::getInstance()->cam->x);
    if((x < 0) || (x > (cWorld::getInstance()->config->displaywidth - cWorld::getInstance()->cam->x))) {
        isalive=false;
    }

}
