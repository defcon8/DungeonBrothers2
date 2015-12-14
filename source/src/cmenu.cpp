#include "SDL2/SDL_ttf.h"
#include <SDL2/SDL_image.h>
#include "../include/cmenu.h"
#include "../include/cconfig.h"
#include "../include/cworld.h"
#include "../include/crenderer.h"



cMenu::cMenu()
{
	TRACE("Construct","cMenu()");
	init();
	ttfInit();
	createSurface();
	buildMenu();
}

void cMenu::ttfInit(){
	TTF_Init();
    ttffont = TTF_OpenFont("Fonts\\BeckerBlackNF.ttf", 20);

    if (ttffont == NULL) {
        TRACE("TTF", "Can't load font: %s",SDL_GetError());
    }else{
    	TRACE("TTF", "Font succesfully loaded.");
	}

}

cMenu::~cMenu()
{
	TRACE("Deconstruct","cMenu()");
	if(texture != NULL) SDL_DestroyTexture(texture);
	if(surface != NULL) SDL_FreeSurface(surface);
	if(background != NULL) SDL_FreeSurface(background);
}

void cMenu::init(){
	texture = NULL;
	surface = NULL;
	selectedmenuitem=0;
	black.r=0;
    black.g=0;
    black.b=0;
    green.r=0;
    green.g=255;
    green.b=0;
    yellow.r=255;
    yellow.g=255;
    yellow.b=0;
}

void cMenu::createSurface(){
	
	Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    rmask = 0xff000000;
	    gmask = 0x00ff0000;
	    bmask = 0x0000ff00;
	    amask = 0x000000ff;
	#else
	    rmask = 0x000000ff;
	    gmask = 0x0000ff00;
	    bmask = 0x00ff0000;
	    amask = 0xff000000;
	#endif

	background = IMG_Load("GFX/menu.bmp");
    surface = SDL_CreateRGBSurface(0, cWorld::getInstance()->config->displaywidth, cWorld::getInstance()->config->displayheight, 32, rmask, gmask, bmask, amask);	
}

void cMenu::buildMenu()
{
	TRACE("Menu","buildMenu()");

    cMenuItem* item1 = new cMenuItem();
    cMenuItem* item2 = new cMenuItem();
    cMenuItem* item3 = new cMenuItem();
    cMenuItem* item3s1 = new cMenuItem();
    cMenuItem* item3s2 = new cMenuItem();
    cMenuItem* item3s3 = new cMenuItem();
    cMenuItem* item4 = new cMenuItem();

    item1->name = "Start Game";
    item1->actionid = 0;

    item2->name = "Editor";
    item2->actionid = 100;

    item3->name = "Options";
    item3->actionid = 200;

    item3s1->name = "Return to Main Menu";
    item3->menuitems.push_back(*item3s1);
    item3s2->name = "Audio";
    item3s2->actionid = 202;
    item3->menuitems.push_back(*item3s2);
    item3s3->name = "Video";
    item3s3->actionid = 203;
    item3->menuitems.push_back(*item3s3);
    item4->name = "Exit";
    item4->actionid = 300;

    menuitems.push_back(*item1);
    menuitems.push_back(*item2);
    menuitems.push_back(*item3);
    menuitems.push_back(*item4);
}

void cMenu::select(){
	
	 //see if the selected menu item has submenuitems
    vector<cMenuItem> *currentmenuitems;
    bool atrootlevel=true;
    //Is the the viewer in topmenu level?
    if(menupath.size() == 0) {
        currentmenuitems = &menuitems;
    }else{
        currentmenuitems = &menupath.front().menuitems;
        atrootlevel=false;
    }

    TRACE("Menu","Selected: %s", currentmenuitems->at(selectedmenuitem).name.c_str());

    int submenuitems = currentmenuitems->at(selectedmenuitem).menuitems.size();
    TRACE("Menu","Subitems: %d",submenuitems);

    if(submenuitems > 0) {
       menupath.push_back(currentmenuitems->at(selectedmenuitem));
        selectedmenuitem = 0; //Reset to top item
    } else {
        //Do action for this menu item.
        if(!atrootlevel && (selectedmenuitem==0)) {
            // Go up one menu level
            menupath.pop_back();
            return;
        } else {
            menuActionDispatcher(currentmenuitems->at(selectedmenuitem).actionid);
        }
    }
	
}

void cMenu::move(int direction){
	switch(direction){
		case 0:
			//up
			TRACE("Events","menu Up");
			 if(menupath.size() == 0) {
                    //main menu
                    selectedmenuitem = (selectedmenuitem > 0 ? selectedmenuitem-1 : (menuitems.size()-1));
            } else {
                    // sub menu
                    selectedmenuitem = (selectedmenuitem > 0 ? selectedmenuitem-1 : (menupath.back().menuitems.size()-1));
            }
			break;
		case 1:
			//down
			TRACE("Events","Menu Down");
			if(menupath.size() == 0) {
                //main menu
                selectedmenuitem = (selectedmenuitem < (menuitems.size()-1) ? selectedmenuitem+1 : 0);
            } else {
                // sub menu
                selectedmenuitem = (selectedmenuitem < (menupath.back().menuitems.size()-1) ? selectedmenuitem+1 : 0);
            }
	}
	
}

void cMenu::menuActionDispatcher(int id)
{
    TRACE("Menu","menuActionDispatcher: %d",id);
    switch(id) {
    case 0:
        cWorld::getInstance()->gamemode = MODE_GAME;
        break;
    case 100:
        cWorld::getInstance()->gamemode = MODE_EDIT;
        break;
    case 300:
        menuActionExit();
        break;
    }
}

void cMenu::menuActionExit()
{
    cWorld::getInstance()->done=true;
}


void cMenu::render(){	
	/* Clear surface */
	SDL_FillRect(surface, NULL, 0x000000);
	
	/* Blit background */
	SDL_BlitSurface(background, NULL, surface, NULL);
	
    //Is the the viewer in topmenu level?
    if(menupath.size() == 0) {
        currentmenuitems = &menuitems;
    } else {
        currentmenuitems = &menupath.front().menuitems;
    }

	
    int item=0;
    for (std::vector<cMenuItem>::iterator it = currentmenuitems->begin(); it != currentmenuitems->end(); it++) {
        SDL_Rect itemlocation;
        itemlocation.w = 200;
        itemlocation.h = 50;
        itemlocation.x = 200;
        itemlocation.y = 200+(item*25);

        SDL_Color itemcolor = (item == selectedmenuitem ? yellow : green);
        SDL_Surface* text = TTF_RenderText_Shaded(ttffont, it->name.c_str(), itemcolor, black);
        SDL_SetColorKey(text, SDL_TRUE, SDL_MapRGB(text->format, 0, 0, 0));
        SDL_BlitSurface(text, NULL, surface, &itemlocation);
        SDL_FreeSurface(text);
        item++;
    }
    
    //Color key image
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        
    //Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(cWorld::getInstance()->renderer->handle, surface);
	
	/* Give the desired part to renderer */
	if(SDL_RenderCopy(cWorld::getInstance()->renderer->handle, texture, NULL, NULL) < 0){
		TRACE("Render","SDL_RenderCopy failed: %s\n", SDL_GetError());
	}
	
	SDL_DestroyTexture(texture);
	
}
