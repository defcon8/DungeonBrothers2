#include <stdio.h>
#include <list>
#include "../include/cdebug.h"
#include "../include/ccamera.h"
#include "../include/cmenu.h"
#include "../include/cworld.h"
#include "../include/clevel.h"
#include "../include/cleveleditor.h"
#include "../include/csprite.h"
#include "../include/crenderer.h"
#include "../include/cgame.h"
#include "../include/ilevelbackground.h"

cGame::cGame()
{
	cWorld::getInstance()->init();
}

cGame::~cGame()
{
	TRACE("Deconstruct","cGame()");
	dispose();
}

void cGame::run()
{
	if(!init()) return;
	TRACE("Init","Init finished, enter game loop.");

	while(!cWorld::getInstance()->done){

        //Handle events
		eventLoop();

		/* Update level object Ai */
		update();
		
		/* Present frame */
		cWorld::getInstance()->cam->cameraMovement();
		render();
        
		SDL_Delay(10);
	} // End while
}

void cGame::render(){
	/* Clear window */ 
	SDL_RenderClear(cWorld::getInstance()->renderer->handle);
	
	switch(cWorld::getInstance()->gamemode){
		case MODE_GAME:
			renderGameMode();
			break;
		case MODE_MENU:
			renderMenuMode();
			break;
		case MODE_EDIT:
			renderEditMode();
			break;
	
	}
	
	/* Render to window */
	SDL_RenderPresent(cWorld::getInstance()->renderer->handle);
}

void cGame::renderMenuMode(){
	cWorld::getInstance()->menu->render();
}


void cGame::renderEditMode(){
	cWorld::getInstance()->leveleditor->render();
}

void cGame::renderGameMode(){
	if(cWorld::getInstance()->getRenderLevel()){
		renderBackgrounds();
		renderObjects();
	} // End if
}

void cGame::update(){
	updateObjects();
}

void cGame::renderBackgrounds()
{
        list<iLevelBackground*>::iterator object = cWorld::getInstance()->level->backgrounds.begin();
        for (object = cWorld::getInstance()->level->backgrounds.begin(); object != cWorld::getInstance()->level->backgrounds.end();++object) {
			(*object)->render(cWorld::getInstance()->cam->x,cWorld::getInstance()->cam->y);	 
        }
}

void cGame::renderObjects()
{
		list<iLevelObject*>::iterator object = cWorld::getInstance()->level->objects.begin();
        for (object = cWorld::getInstance()->level->objects.begin(); object != cWorld::getInstance()->level->objects.end();++object) {
			(*object)->render();	 
        }	
}

void cGame::updateObjects()
{
		list<iLevelObject*>::iterator object = cWorld::getInstance()->level->objects.begin();
        for (object = cWorld::getInstance()->level->objects.begin(); object != cWorld::getInstance()->level->objects.end();) {
        	
        	//If object is dead.. 
        	if(!(*object)->isAlive()) {
        		iLevelObject *p_deadobject = (*object); //Store pointer to dead object
				cWorld::getInstance()->level->objects.erase(object++); // remove from object list
				delete(p_deadobject); // delete dead object
            } else {
                (*object)->update();
                ++object;
            }	 
        }	
}

bool cGame::init()
{
  TRACE("Init","Start initialize..");
  
  cWorld::getInstance()->level->load("Level1.dat");
  
  return true;
}

void cGame::dispose()
{
  TRACE("Init","cGame::dispose()");
  SDL_Quit();
}

void cGame::commonEvents(SDL_Event *event){
	  switch(event->type) {
	  		case SDL_KEYDOWN:
	  			switch(event->key.keysym.sym) {
            		case SDLK_ESCAPE:
            			TRACE("Events","Toggle between menu and game mode.");
            			/* Togle between game and menu mode */
            			if(cWorld::getInstance()->gamemode == MODE_GAME){
            				cWorld::getInstance()->gamemode = MODE_MENU;
            				TRACE("Events","MENU Mode.");
						}else if(cWorld::getInstance()->gamemode == MODE_MENU){
							cWorld::getInstance()->gamemode = MODE_GAME;
							TRACE("Events","GAME Mode.");
						}else if(cWorld::getInstance()->gamemode == MODE_EDIT){
							cWorld::getInstance()->gamemode = MODE_MENU;
							TRACE("Events","MENU Mode.");
						}
            			break;
            		case SDLK_x:
	                	cWorld::getInstance()->done = true;
	                	break;
	            }
	  			break;
	  		case SDL_WINDOWEVENT:
	  			switch (event->window.event) {
	  				case SDL_WINDOWEVENT_CLOSE:
			            cWorld::getInstance()->done = true;
			            break;
				  }
				  break;
            case SDL_QUIT:
                done = true;
                break;
            default:
                break;
        }   // End switch
}

void cGame::menuModeEvents(SDL_Event *event){
	switch (event->type) {
        	case SDL_KEYDOWN:
        		switch(event->key.keysym.sym) {
        			case SDLK_UP:
		                TRACE("Events","Keydown UP");
		                cWorld::getInstance()->menu->move(0);
		                break;
		            case SDLK_DOWN:
		                TRACE("Events","Keydown DOWN");
		                cWorld::getInstance()->menu->move(1);
		                break;  
					case SDLK_RETURN:
					 	TRACE("Events","Keydown RETURN");
					 	cWorld::getInstance()->menu->select();
					 	break;
        		}
				
				break;
	}
}

void cGame::gameModeEvents(SDL_Event *event){
        switch (event->type) {
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
	            case SDLK_UP:
	                TRACE("Events","Keydown UP");
	                cWorld::getInstance()->level->playerobject->moveDirection(1,true);
	                break;
	            case SDLK_RIGHT:
	                TRACE("Events","Keydown RIGHT");
	                cWorld::getInstance()->level->playerobject->moveDirection(2,true);
	                break;
	            case SDLK_DOWN :
	                TRACE("Events","Keydown DOWN");
	                cWorld::getInstance()->level->playerobject->moveDirection(3,true);
	                break;
	            case SDLK_LEFT:
	                TRACE("Events","Keydown LEFT");
	                cWorld::getInstance()->level->playerobject->moveDirection(4,true);
	                break;
	            case SDLK_SPACE:
	                if(!cWorld::getInstance()->level->playerobject->isjumping)
	                    cWorld::getInstance()->level->playerobject->jump();
	                TRACE("Events","Keydown SPACE");
	                break;
	            case SDLK_RALT:
	                cWorld::getInstance()->level->playerobject->fire();
	                TRACE("Events","Keydown R-ALT");
	                break;
	            }
            break;
        case SDL_KEYUP:
            switch(event->key.keysym.sym) {
	            case SDLK_UP: {
	                cWorld::getInstance()->level->playerobject->moveDirection(1,false);
	                TRACE("Events","Keyup UP");
	                break;
	            }
	
	            case SDLK_RIGHT: {
	                cWorld::getInstance()->level->playerobject->moveDirection(2,false);
	                TRACE("Events","Keyup RIGHT");
	                break;
	            }
	            case SDLK_DOWN: {
	                cWorld::getInstance()->level->playerobject->moveDirection(3,false);
	                TRACE("Events","Keyup DOWN");
	                break;
	            }
	            case SDLK_LEFT: {
	                cWorld::getInstance()->level->playerobject->moveDirection(4,false);
	                TRACE("Events","Keyup LEFT");
	                break;
	            }
            }
            break;
        default:
            break;
        }
}

void cGame::eventLoop()
{
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
    	commonEvents(&event);
    	if(cWorld::getInstance()->gamemode == MODE_GAME) gameModeEvents(&event);
        if(cWorld::getInstance()->gamemode == MODE_MENU) menuModeEvents(&event);  
    }   // End while
}

