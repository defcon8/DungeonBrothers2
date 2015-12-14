
#include <cstring>
#include "..\include\cdebug.h"
#include "..\include\cworld.h"
#include "..\include\crenderer.h"
#include "..\include\csprite.h"

Uint32 cSprite::getPixelColor(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

bool cSprite::getSlopes()
{
	TRACE("Slopes","GetSlopes() -> %s", tilesource);

    // This function scans all tiles in the bitmap and detects if a pixel is transparant or not. Information for each pixel row of the sprite is stored in so
    // called slopes which is basicaly a word (32 bit). Each bit in the word tells if the pixels is transparant or not.

    //create data object
    pixelinfo = new spixelinfo*[texture->surfacehandle->w];
    for(int i=0; i < texture->surfacehandle->w; i++)
        pixelinfo[i] = new spixelinfo[texture->surfacehandle->h];

    int pixelcount=0; // just for debugging purposes, count how many pixels are analyzed

    SDL_LockSurface(texture->surfacehandle);
    for(int x=0; x<=texture->surfacehandle->w-1; x++) {
        for(int y=0; y<=texture->surfacehandle->h-1; y++) {

            Uint8 r, g, b; // temporary
            Uint32 pixelcolor = getPixelColor(texture->surfacehandle, x, y);
            SDL_GetRGB(pixelcolor, texture->surfacehandle->format, &r, &g, &b);

            // The position of the bit in the Long (lSlopeRow) is the same as the pixel position in the row to be scanned
            if((r==colorkeyr) && (g==colorkeyg) && (b==colorkeyb)) { // if the color is the keycolor then it it transparant...
                //Transparant Pixel (air...)
                pixelinfo[x][y].transparant = 0;
            } else {
                //Object (floor, wall, ground whatever..) we can stand on it
                pixelinfo[x][y].transparant = 1;
            }

            pixelcount++;
        }
    }
    SDL_UnlockSurface(texture->surfacehandle);
    SDL_FreeSurface(texture->surfacehandle);

    TRACE("Slopes","Pixels analyzed: %d", pixelcount);
}

void cSprite::load(const char *file)
{
    string path("GFX\\");
    string filename(file,20);
    filename.insert(0,path);

    TRACE("Sprite","Load file: %s", filename.c_str());
    texture = new cTexture();
    texture->loadFromFile(filename);
    
    //Store the filename localy in chTileSource, we need it later when saving level to disk.
    memcpy(&tilesource[0],file,16);
}

char* cSprite::getTileSource()
{
    return &tilesource[0];
}

void cSprite::setColorKey(int r, int g, int b)
{
    colorkeyr = r;
    colorkeyg = g;
    colorkeyb = b;
}

void cSprite::render(int destx, int desty)
{
	render(0, 0, destx, desty);
}

void cSprite::render(int col, int row, int destx, int desty)
{
    /* Part of the bitmap that we want to draw */
    SDL_Rect source;
    source.x = spritewidthoffset+(col*(spritewidth+spritespacer));
    source.y = spriteheightoffset+(row*(spriteheight+spritespacer));
    source.w = spritewidth;
    source.h = spriteheight;

    /* Part of the screen we want to draw the sprite to */
    SDL_Rect destination;
    destination.x = destx - scrolloffset;
    destination.y = desty;
    destination.w = spritewidth;
    destination.h = spriteheight;

	if(usebuffer){
		/* Set buffer texture as temporary render target */
		if(SDL_SetRenderTarget(cWorld::getInstance()->renderer->handle, buffertexture) < 0){
			TRACE("Sprite","SDL_SetRenderTarget failed: %s\n", SDL_GetError());
		}
	}

	/* Give the desired part to renderer */
	if(SDL_RenderCopy(cWorld::getInstance()->renderer->handle, texture->handle, &source, &destination) < 0){
		TRACE("Render","SDL_RenderCopy failed: %s\n", SDL_GetError());
	}
    
    if(usebuffer){
		/* Reset to default rendering target */
		if(SDL_SetRenderTarget(cWorld::getInstance()->renderer->handle, NULL) < 0){
			TRACE("Sprite","SDL_SetRenderTarget failed: %s\n", SDL_GetError());
		}	
	}
}

cSprite::cSprite()
{
	TRACE("Construct","cSprite()");
	texture = NULL;
	buffertexture = NULL;
	usebuffer = false;
    init();
}

cSprite::cSprite(SDL_Texture* _buffertexture){
	TRACE("Construct","cSprite()");
	texture = NULL;
	buffertexture = _buffertexture;
	usebuffer = true;
	SDL_SetTextureBlendMode(buffertexture, SDL_BLENDMODE_BLEND);
	init();
}

cSprite::~cSprite()
{
	TRACE("Deconstruct","cSprite()");
	if(buffertexture != NULL) SDL_DestroyTexture(buffertexture);
	if(texture != NULL) delete(texture);
	
}

void cSprite::init()
{
    spritespacer=0;
    spriteheight=0;
    spritewidth=0;
    spriteheightoffset=0;
    spritewidthoffset=0;
    scrolloffset=0;
}

