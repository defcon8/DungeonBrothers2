#include <cstddef>
#include "../include/cdebug.h"
#include "../include/cconfig.h"
#include "../include/ccamera.h"
#include "../include/cmenu.h"
#include "../include/cworld.h"
#include "../include/cwindow.h"
#include "../include/crenderer.h"
#include "../include/clevel.h"
#include "../include/cleveleditor.h"
#include "../include/cfunctions.h"



cWorld* cWorld::instance = NULL;

cWorld::cWorld()
{
	TRACE("Construct","cWorld()");
}

cWorld::~cWorld()
{
	TRACE("Deconstruct","cWorld()");
}

cWorld* cWorld::getInstance()
{
    if(!instance) {
    	TRACE("Construct","Create world instance");
		instance = new cWorld();
    }
    return instance;
}

void cWorld::init(){
	//This is a dummy routine. Which is used to construct the singleton object on application run.
	TRACE("Init","World space initialized.");
	config = new cConfig();
	cam = new cCamera();
	menu = new cMenu();
	window = new cWindow();
	renderer = new cRenderer();
	level = new cLevel();
	leveleditor = new cLevelEditor();
	functions = new cFunctions();
	
	gamemode = MODE_MENU;
	done = false;
	renderlevel = true;
}

void cWorld::setRenderLevel(bool value){
	renderlevel = value;
}

bool cWorld::getRenderLevel(){
	return renderlevel;
}
