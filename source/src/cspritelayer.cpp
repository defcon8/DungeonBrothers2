#include "../include/cspritelayer.h"
#include "../include/clevel.h"
#include "../include/crenderer.h"
#include "../include/cdebug.h"
#include "../include/cconfig.h"

using namespace std;

void cSpriteLayer::drawPixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 color = SDL_MapRGB(screen->format, r, g, b);
    switch (screen->format->BytesPerPixel) {
    case 1: { // 8-bpp
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
        *bufp = color;
    }
    break;
    case 2: { // 15-bpp or 16-bpp
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
        *bufp = color;
    }
    break;
    case 3: { // 24-bpp mode, usually not used
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
            bufp[0] = color;
            bufp[1] = color >> 8;
            bufp[2] = color >> 16;
        } else {
            bufp[2] = color;
            bufp[1] = color >> 8;
            bufp[0] = color >> 16;
        }
    }
    break;
    case 4: { // 32-bpp
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
        *bufp = color;
    }
    break;
    }
}


bool cSpriteLayer::pixelIsTransparant(int row, int col, int x, int y, int colcount)
{
    // Find out while tile this is.. in the sprite source.

    //Get the start TopLeft position.
    int rowinsource = leveldata[row][col].row;
    int colinsource = leveldata[row][col].index;

    //Get the pixel
    int pixely = rowToYInSpriteSheet(rowinsource) + y;
    int pixelx = colToXInSpriteSheet(colinsource) + x;

    bool solid = (bool)source->pixelinfo[pixelx][pixely].transparant;

    TRACE("pixelIsTransparant","Row: %d  Col: %d  RowInSource: %d  ColInSource: %d  PixelX: %d  PixelY: %d  iX: %d  iY: %d  Solid: %s", row, col, rowinsource, colinsource, pixelx, pixely, x ,y, solid ? "true" : "false");

    return solid;
}

cSpriteLayer::cSpriteLayer(int rows, int cols, int spriteheight, int spritewidth, bool optimize, bool isbuffered, bool usecolorkey, int keyr, int keyg, int keyb)
{
	TRACE("Construct","cSpriteLayer();");

    /**< Initialize variables and setup data object holding the level data */
    initLayer(rows, cols, spriteheight, spritewidth, optimize, isbuffered);

    /**< Setup (source) object that contains the Sprite Sheet. */
    if(isBuffered()) {
    	
    	Uint32 rmask, gmask, bmask, amask;
    	
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

		texture = SDL_CreateTexture(cWorld::getInstance()->renderer->handle, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, getWidth(), getHeight()); 

        source = new cSprite(texture);
    } else {
        source = new cSprite();
    }

    /**< Commmit the Color key for all surfaces. It's also used for slope detection */
    if(usecolorkey) {
        colorkeyr = keyr;
        colorkeyg = keyg;
        colorkeyb = keyb;
        if(usebuffer) {
			source->setColorKey(colorkeyr, colorkeyg, colorkeyb);
		}
	}

    /**< Fill map data with init data */
    initMap();
}

void cSpriteLayer::getSlopes()
{
    source->getSlopes();
}

void cSpriteLayer::clearlayer()
{
    Uint32 * pixels = new Uint32[(colcount*spritewidth) * (rowcount*spriteheight)];
    memset(pixels, 255, (colcount*spritewidth) * (rowcount*spriteheight) * sizeof(Uint32));
    SDL_UpdateTexture(texture, NULL, pixels, (colcount*spritewidth) * sizeof(Uint32));
}


void cSpriteLayer::initLayer(int rows, int cols, int _spriteheight, int _spritewidth, bool optimize, bool isbuffered)
{
	texture = NULL;
	source = NULL;
    spriteheight = _spriteheight;
    spritewidth = _spritewidth;
    x = 0;
    y = 0;
    rowcount = rows;
    colcount = cols;
    colorkeyr = 0;
    colorkeyg = 0;
    colorkeyb = 0;
    createbuffer = true;
    optmizelayer = optimize;
    usebuffer = isbuffered;

    //create data object
    leveldata = new slevelblock*[rows];
    for(int i=0; i < rows; i++)
        leveldata[i] = new slevelblock[cols];
}

void cSpriteLayer::setSpriteHeight(int pixels)
{
    spriteheight = pixels;
}

int cSpriteLayer::getSpriteHeight()
{
    return spriteheight;
}

void cSpriteLayer::setSpriteWidth(int pixels)
{
    spritewidth = pixels;
}

int cSpriteLayer::getSpriteWidth()
{
    return spritewidth;
}

Uint8 cSpriteLayer::returnSpriteFlags(int row, int col)
{
    return leveldata[row][col].flags;
}

void cSpriteLayer::initMap()
{
    for (int row = 0; row < rowcount; row++) {
        for (int col = 0; col < colcount; col++) {
            leveldata[row][col].index = 1;
            leveldata[row][col].row = 1;
            leveldata[row][col].type = EMPTY;
            leveldata[row][col].flags = (unsigned char)~(PLATFORM | DAMAGE);
        }
    }
}

cSpriteLayer::~cSpriteLayer()
{
	TRACE("Deconstruct","cSpriteLayer();");
	if(texture != NULL) SDL_DestroyTexture(texture);
	if(source != NULL) delete(source);
	
}

void cSpriteLayer::render(signed int camx, signed int camy)
{
	
	if(isBuffered() && texture != NULL && !createbuffer){
		
		SDL_Rect SrcR;
		SrcR.x = 0;
  		SrcR.y = 0;
  		SrcR.w = getWidth();
  		SrcR.h = getHeight();
		
		SDL_Rect DestR;
		DestR.x = camx;
  		DestR.y = camy;
  		DestR.w = getWidth();
  		DestR.h = getHeight();

		
		
		/* The buffer texture is filled, use this instead of rendering all tiles again */
		if(SDL_RenderCopy(cWorld::getInstance()->renderer->handle, texture, &SrcR, &DestR) < 0){
			TRACE("Render","SDL_RenderCopy failed: %s\n", SDL_GetError());
		}
		return;
	}
	
	
    // Don't draw things that are outside the view.
    int startcol=0;
    int startrow=0;
    int endcol=colcount;
    int endrow=rowcount;

    if(optmizelayer) {
        startcol = xToCol(-camx);
        endcol = xToCol((-camx)+cWorld::getInstance()->config->displaywidth); 
        startrow = yToRow(-camy);
        endrow = yToRow((-camy)+cWorld::getInstance()->config->displayheight); 

        // Protect drawing level outside its boundaries
        if(startcol<0) {
            startcol=0;
        }
        if(startrow<0) {
            startrow=0;
        }
        if(endcol>colcount) {
            endcol=colcount;
        }
        if(endrow>rowcount) {
            endrow=rowcount;
        }
    }

    // Loop
    for (int row = startrow; row < endrow; row++) {
        for (int col = startcol; col < endcol; col++) {
            if(leveldata[row][col].type != EMPTY)
                source->render(leveldata[row][col].index, leveldata[row][col].row, (colToX(col)+camx)+x, (rowToY(row)+camy)+y);
        }
    }

}

int cSpriteLayer::getTotalRows()
{
    return rowcount;
}
int cSpriteLayer::getTotalCols()
{
    return colcount;
}

signed int cSpriteLayer::colToXInSpriteSheet(signed int col)
{
    return (col*(spritewidth+source->spritespacer)) + source->spritespacer; // See below
}
signed int cSpriteLayer::rowToYInSpriteSheet(signed int row)
{
    TRACE("pixelIsTransparant","iSpriteHeightOffset: %d",source->spriteheightoffset);
    return source->spriteheightoffset + (row*(source->spriteheight+source->spritespacer)); //TODO: The last value (iSpriteSpace isnt correct, it should be heightoffset, but somehow it doesnt work)
}

signed int cSpriteLayer::colToX(signed int col)
{
    return (col*spritewidth);
}
signed int cSpriteLayer::rowToY(signed int row)
{
    return (row*spriteheight);
}
signed int cSpriteLayer::xToCol(signed int width)
{
    return (width/spritewidth);
}
signed int cSpriteLayer::yToRow(signed int height)
{
    return (height/spriteheight);
}
signed int cSpriteLayer::getWidth()
{
    return colcount * spritewidth;
}
signed int cSpriteLayer::getHeight()
{
    return rowcount * spriteheight;
}
bool cSpriteLayer::isBuffered()
{
    return usebuffer;
}

