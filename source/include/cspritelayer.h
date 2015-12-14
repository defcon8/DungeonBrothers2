#ifndef CSPRITELAYER_H
#define CSPRITELAYER_H

#include "../include/csprite.h"
#include "../include/cworld.h"
#include "../include/ilevelbackground.h"


#include <SDL2/SDL.h>

//Sprite Types
#define EMPTY 0
#define SPRITE  1

//Sprite Flags
#define WALL 1
#define ENEMYWALL 2
#define PLATFORM 4
#define USELIGHT 8
#define OVERLAP 16
#define ANBACK 32
#define ANFRONT 64
#define WATER 128
#define DAMAGE 256
#define CONVL 512
#define CONVR 1024
#define TURN 2048
#define DESTROYABLE 4096

class cSpriteLayer : public iLevelBackground
{
	private:
		
		struct slevelblock {
	        Uint8 source;
	        Uint8 row;
	        Uint8 index;
	        Uint8 type;
	        Uint8 animations;
	        Uint8 aniframestep;
	        Uint8 collide;
	        Uint8 flags;
    	};
    	
    	int spritewidth, spriteheight;
	    int rowcount, colcount;
	    bool optmizelayer;
	    bool usebuffer;
	    bool usecolorkey;
	    int colorkeyr, colorkeyg, colorkeyb;
    	
		//Methods
    	void initMap();
    	void drawPixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b);
    	void initLayer(int rows, int cols, int spriteheight, int spritewidth, bool optimize, bool isbuffered);
		
		
	public:
		cSpriteLayer(int rows, int cols, int spriteheight, int spritewidth, bool optimize, bool isbuffered, bool usecolorkey, int keyr, int keyg, int keyb); /**< Constructor to render directly to main screen */
    	~cSpriteLayer();
    	
	    //Data Objects
	    cSprite *source;
	    virtual void render(signed int camx, signed int camy);
	    SDL_Texture* texture;
	    slevelblock **leveldata;	
    	
    	 //Members
	    int x,y;
	
	    //Methods
	    Uint8 returnSpriteFlags(int row, int col);
	    void setSpriteWidth(int pixels);
	    void getSlopes();
	    int getSpriteWidth();
	    void setSpriteHeight(int pixels);
	    void clearlayer();
	    int getSpriteHeight();
	    bool pixelIsTransparant(int row, int col, int x, int y, int colcount);
	    int getTotalRows();
	    int getTotalCols();
	    int getWidth();
	    int getHeight();
	    signed int colToXInSpriteSheet(signed int col);
	    signed int rowToYInSpriteSheet(signed int row);
	    int xToCol(signed int width);         		/**< Returns the col number of the given width in pixels */
	    int yToRow(signed int height);       		/**< Returns the row number of the given height in pixels */
	    int colToX(signed int col);          		 /**< Returns the width in pixels of the col number */
	    int rowToY(signed int row);         		 /**< Returns the height in pixels of the row number */
	    bool isBuffered();                         /**< Returns if the surface is a buffered or not. */
	    bool createbuffer;					
    	
    	
    	
	protected:
};

#endif
