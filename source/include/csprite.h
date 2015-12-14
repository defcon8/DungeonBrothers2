#ifndef CSPRITE_H
#define CSPRITE_H

#include <SDL2/SDL.h>
#include "../include/cspriteslope.h"
#include "../include/ilevelbackground.h"
#include "../include/ctexture.h"


class cSprite : public iLevelBackground
{
	
	private:
		int colorkeyr, colorkeyg, colorkeyb;
    	char tilesource[16];
    	void init();
		bool usebuffer;

    	cTexture* texture;
    	SDL_Texture* buffertexture;
    	
    	struct spixelinfo{
        	int transparant;
    	};
		
	
	public:
		cSprite();
		cSprite(SDL_Texture* _buffertexture);							/* Altenative constructor, used when rendering to an Texture instead of Screen */
		~cSprite();
		
		spixelinfo **pixelinfo;

    	int scrolloffset, spritewidth, spriteheight, spritewidthoffset ,spriteheightoffset, spritespacer;
    	cSpriteSlope* slopeleft, *sloperight, *slopetop, *slopebottom;
    	
    	void load(const char *file);
    	void render(int destx, int desty); 								/* Simplified renderpass, for single image sprite */
    	void render(int col, int row, int destx, int desty);			/* Rednderpass for multi image sprite */
		Uint32 getPixelColor(SDL_Surface *surface, int x, int y);
		bool getSlopes();
    	void scroll();
		void setSpriteWidth(int pixels);
    	void setSpriteHeight(int pixels);
    	void setSpriteWidthOffset(int pixels);
    	void setSpriteHeightOffset(int pixels);
    	void setSpriteSpacer(int pixels);
    	void setColorKey(int r, int g, int b);
    	char* getTileSource();
		
	protected:
};

#endif
