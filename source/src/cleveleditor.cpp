#include <math.h>
#include "../include/cdebug.h"
#include "../include/ccamera.h"
#include "../include/cconfig.h"
#include "../include/cworld.h"
#include "../include/clevel.h"
#include "../include/cleveleditor.h"
#include "../include/crenderer.h"
#include "../include/csprite.h"
#include <SDL2/SDL.h>

cLevelEditor::cLevelEditor()
{
	TRACE("Construct","cLevelEditor()");
	init();
	loadPickerSource("GFX\blocks1.bmp");
	
}

cLevelEditor::~cLevelEditor()
{
	TRACE("Deconstruct","cLevelEditor()");
}

void cLevelEditor::loadPickerSource(string filename){
	//Todo: needs to be given by user input
	rowsinsource = 11;
	colsinsource = 18;
	tilewidth = 32;
	tileheight = 32;
	
	pickersource = new cSprite();
	pickersource->spritespacer = 2;
	pickersource->load("blocks1.bmp");
	pickersource->spritewidthoffset = 2;
	pickersource->spriteheightoffset = 2;
	pickersource->spriteheight = tileheight;
	pickersource->spritewidth = tilewidth;
}

void cLevelEditor::init(){
	showspritepalet = false;
	pickercols = 3;
	tilewidth = 0;
	tileheight = 0;
	scrollbarwidth = 20;
	rowsinsource = 0;
	colsinsource = 0;
}

void cLevelEditor::drawSpritePicker(){
	drawSpritePickerPanel();
	drawSpritePickerScrollBar();
	drawSpritePickerTiles();
}

void cLevelEditor::drawSpritePickerTiles(){	
	
	int pickerdialogwidth = (pickercols*tilewidth) + scrollbarwidth;
	int initialx = cWorld::getInstance()->config->displaywidth - pickerdialogwidth;
	int tilex = initialx;
	int tiley = 0;
	int thistile=0;
	
	for(int sourcerow=0; sourcerow < (rowsinsource-1); sourcerow++){
		for(int sourcecol=0; sourcecol < (colsinsource-1); sourcecol++){
			
			// Draw the tile at the given position.
			pickersource->render(sourcecol, sourcerow, tilex , tiley);
			
			if((thistile+1) % pickercols == 0){
				//reset tilex
				tilex = initialx;
				//shift tiley to next row
				tiley += tileheight;
			}else{
				//shift tilex to next col
				tilex += tilewidth;
			}
			thistile++;
		}
	}
}

void cLevelEditor::drawSpritePickerScrollBar(){
	/* Sprite picker plane backcolor = red */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, 128, 128, 128, 255);
	
	/* Create position rectangle */
	SDL_Rect r;
    r.x = cWorld::getInstance()->config->displaywidth - scrollbarwidth;
    r.y = 0;
    r.w = scrollbarwidth;
    r.h = 100;
    
	SDL_RenderFillRect(cWorld::getInstance()->renderer->handle, &r);
	
	/* Set default render color back to black */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, 0, 0, 0, 255);
}

void cLevelEditor::drawSpritePickerPanel(){
	int pickerdialogwidth = (pickercols*tilewidth) + scrollbarwidth;
	
	/* Sprite picker plane backcolor = darkgray */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, 32, 32, 32, 255);
	
	/* Create position rectangle */
	SDL_Rect r;
    r.x = cWorld::getInstance()->config->displaywidth - pickerdialogwidth;
    r.y = 0;
    r.w = pickerdialogwidth;
    r.h = cWorld::getInstance()->config->displayheight;
    
	SDL_RenderFillRect(cWorld::getInstance()->renderer->handle, &r);
	
	/* Set default render color back to black */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, 0, 0, 0, 255);
}

//void cLevelEditor::loadSpritePicker(){
//    spritepicker = new cSpriteLayer(sourcerows, sourcecols, spriteheight, spritewidth,false,false,false,0,0,0);
//    spritepicker->source->spritespacer = 2;
//    spritepicker->source->load(tilesource);
//    spritepicker->source->spritewidthoffset = 0;
//    spritepicker->source->spriteheightoffset = 0;
//    spritepicker->source->spriteheight = spriteheight;
//    spritepicker->source->spritewidth = spritewidth;
//
//	int i
//
//    for (int col = 0; col < sourcecols; col++) {
//        for (int row = 0; row < sourcerows; row++) {
//            world->spritepicker->leveldata[row][col].type = SPRITE;
//            world->spritepicker->leveldata[row][col].row = row;
//            world->spritepicker->leveldata[row][col].index = col;
//        }
//    }
//}


int cLevelEditor::getTileCol(int x, int tilewidth)
{
    return static_cast<int>(floor(x/tilewidth));
}
int cLevelEditor::getTileRow(int y, int tileheight)
{
    return static_cast<int>(floor(y/tileheight));
}

void cLevelEditor::render(){
	drawSpritePicker();
	drawSpriteSelector();
}

void cLevelEditor::drawSpriteSelector(){
	//	// Get position of mouse and calculate the according position in tiles/rows
	int mousex, mousey;
	int mousebuttons = SDL_GetMouseState(&mousex, &mousey);
	int tilewidth = cWorld::getInstance()->level->levellayer->getSpriteWidth();
	int tileheight = cWorld::getInstance()->level->levellayer->getSpriteHeight();
	int tilecol = getTileCol(mousex, tilewidth);
	int tilerow = getTileRow(mousey, tileheight);
	int rectx = (tilecol * tilewidth);
	int recty = (tilerow * tileheight);
	int xoffset=0,yoffset=0;

	 // Because of the screenscrolling you need to know the offset in pixels before we can exactly calculate on which tile the mouse cursor is. But
	// we dont want to do this when the sprite picker is shown, because that is always statis align from the upper left corner (0,0).
	if (!showspritepalet) {
	    xoffset = (-(cWorld::getInstance()->cam->x)%tilewidth);
	    yoffset = (-(cWorld::getInstance()->cam->y)%tileheight);
	}
	
	/* Draw the sprite selector square */
	drawRectangle(rectx-xoffset, recty-yoffset, tilewidth, tileheight, 255, 0, 0, 128);
}


void cLevelEditor::drawRectangle(int left, int top, int width, int height, int r, int g, int b, int a){
	/* Sprite picker plane backcolor = red */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, r, g, b, a);
	
	/* Create position rectangle */
	SDL_Rect rect;
    rect.x = left;
    rect.y = top;
    rect.w = width;
    rect.h = height;
    
	SDL_RenderFillRect(cWorld::getInstance()->renderer->handle, &rect);
	
	/* Set default render color back to black */
	SDL_SetRenderDrawColor(cWorld::getInstance()->renderer->handle, 0, 0, 0, 255);
}

void cLevelEditor::render2(){
//	// Get position of mouse and calculate the according position in tiles/rows
//    int mousex, mousey;
//    int mousebuttons = SDL_GetMouseState(&mousex, &mousey);
//    int tilewidth = cWorld::getInstance()->level->levellayer->getSpriteWidth();
//    int tileheight = cWorld::getInstance()->level->levellayer->getSpriteHeight();
//    int tilecol = getTileCol(mousex, tilewidth);
//    int tilerow = getTileRow(mousey, tileheight);
//    int rectx = (tilecol * tilewidth);
//    int recty = (tilerow * tileheight);
//    int xoffset=0,yoffset=0;
//
//    // Because of the screenscrolling you need to know the offset in pixels before we can exactly calculate on which tile the mouse cursor is. But
//    // we dont want to do this when the sprite picker is shown, because that is always statis align from the upper left corner (0,0).
//    if (!showspritepalet) {
//        xoffset = (-(cWorld::getInstance()->cam->x)%tilewidth);
//        yoffset = (-(cWorld::getInstance()->cam->y)%tileheight);
//    }
//
//    //Recalculate the real tile using the camera offset
//    tilecol = getTileCol(mousex - cWorld::getInstance()->cam->x-xoffset, tilewidth);
//    tilerow = getTileRow(mousey - cWorld::getInstance()->cam->y-yoffset, tileheight);
//
//    if(showspritepalet) { // Need to display the sprite picker
//        spritepicker->render(0,0);  //render this at fixed position.
//        switch(mousebuttons) {
//        case 1:
//            //Left button, selects the source sprite
//            int pickedtilecol = getTileCol(mousex, tilewidth);
//            int pickedtilerow = getTileRow(mousey, tileheight);
//            world->pencil->sourcetilerow = pickedtilerow;
//            world->pencil->sourcetilecol = pickedtilecol;
//
//            spritepalet = !spritepalet; // Hide sprite palet
//            cWorld::getInstance()->renderlevel = !renderlevel; // Show level again
//            break;
//        }
//    } else {
//        // The is in paint mode
//        if((tilerow < cWorld::getInstance()->level->levellayer->getTotalRows()) && (tilecol < cWorld::getInstance()->level->levellayer->getTotalCols()) && (tilerow > -1) && (tilecol > -1)) {
//            switch(mousebuttons) {
//            case 1:
//                //Left button, Draw
//                cWorld::getInstance()->level->leveldata[tilerow][tilecol].index = world->pencil->sourcetilecol;
//                cWorld::getInstance()->level->leveldata[tilerow][tilecol].row = world->pencil->sourcetilerow;
//                cWorld::getInstance()->level->leveldata[tilerow][tilecol].type = SPRITE;
//                break;
//            case 4:
//                //Right button, Clear
//                cWorld::getInstance()->level->levellayer->leveldata[tilerow][tilecol].index=0;
//                cWorld::getInstance()->level->levellayer->leveldata[tilerow][tilecol].type=EMPTY;
//                break;
//            }
//        }
//    }
//
//    //Draw tile placeholder aka the mouse pointer
//    //drawRectangle(rectx-xoffset, recty-yoffset, tilewidth, tileheight, 0xFFFFFF);
}
