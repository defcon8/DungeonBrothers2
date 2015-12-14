#include "../include/ccamera.h"
#include "../include/cconfig.h"
#include "../include/cdebug.h"
#include "../include/cworld.h"
#include "../include/clevel.h"


cCamera::cCamera(){
	TRACE("Construct","cCamera()");
	dty=0;
	dtx=0;
	velocity=1;
	up=1;
	right=2;
	down=3;
	left=4;
	none=5;
	x=0;
	y=0;
	direction=none;
}

cCamera::~cCamera(){
	TRACE("Deconstruct","cCamera()");
}

void cCamera::cameraMovement()
{
	
    //Do Camera movement (manual movement, for example in level editor)
    switch(direction) {
    case 1:
        y += 1;
        break;
    case 2:
        x -= 1;
        break;
    case 3:
        y -= 1;
        break;
    case 4:
        x += 1;
        break;
    }


	//Automatic camera movement during gameplay
    if(cWorld::getInstance()->gamemode == MODE_GAME) { // Dont move camera automaticly when in edit mode because we want to move by mouse

        //Let the camera follow the players position. If the camera position is not optimal then increase or decrease the camera position by 1 to have a smooth scrolling effect. Do not follow
        //when the user is jumping..this is kind a hectic.

        //Left/Right
        pos = -(cWorld::getInstance()->level->playerobject->x - (cWorld::getInstance()->config->displaywidth/2) - cWorld::getInstance()->level->playerobject->gfxlayer->getWidth());
		dtx = max(x,pos) - min(x,pos);
		
        if((pos < x) && ((int)(dtx / velocity) > 0 )){
            	x-=velocity; // Let the Camera go left
        }else if((pos > x) && ((int)(dtx / velocity) > 0 )){
        		x+=velocity;         		// Let the Camera go left
        }

        //Up/Down
        if(cWorld::getInstance()->level->playerobject->isjumping)  return;  //Do not update camera on player jump
        
		pos = -(cWorld::getInstance()->level->playerobject->y - (cWorld::getInstance()->config->displayheight/2) - cWorld::getInstance()->level->playerobject->gfxlayer->getHeight());
        dty = max(y,pos) - min(y,pos);
        
		if((pos < y) && ((int)(dty / velocity) > 0 )) {
            	y-=velocity; // Let the Camera go up
        }else if((pos > y) && ((int)(dty / velocity) > 0)){
            	y+=velocity; // Let the Camera go down
        } else {
            // The camera is in optimal position
        }


    }
    
}
