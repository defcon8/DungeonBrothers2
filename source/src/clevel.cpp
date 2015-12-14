#include "../include/cdebug.h"
#include "../include/clevel.h"
#include "../include/cworld.h"
#include "../include/ctmxdoc.h"
#include <fstream>

cLevel::cLevel()
{
	TRACE("Construct","cLevel()");
	cTMXDoc *tmx;
	tmx = new cTMXDoc();
	tmx->load();
}

cLevel::~cLevel()
{
	TRACE("Construct","cLevel()");
}

bool cLevel::load(string file)
{
	
	TRACE("Disk","loadLevel()");

    //Background Layer
    backgroundlayer = new cSprite();
    backgroundlayer->load("back.bmp");
    backgroundlayer->spritewidth = 640;
    backgroundlayer->spriteheight = 480;

	/* Add levellayer to backgroundlayers list */
	backgrounds.push_back(backgroundlayer);

    //Level Layer
    char tilesource[16];
    Uint16 levelrows;
    Uint16 levelcols;
    Uint16 spriteheight;
    Uint16 spritewidth;
    Uint16 sourcerows;
    Uint16 sourcecols;
    Uint8 spritespacer;
    Uint8 spritewidthoffset;
    Uint8 spriteheightoffset;
    Uint16 datablocks;

    string path("Data\\");
    file.insert(0,path);

    TRACE("Disk","Open levelfile: %s",file.c_str());

    ifstream load;
    load.open(file.c_str(), ios_base::in | ios_base::binary);

    load.read(reinterpret_cast<char*>(&tilesource),sizeof(tilesource));
    load.read(reinterpret_cast<char*>(&levelrows),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&levelcols),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&spriteheight),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&spritewidth),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&sourcerows),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&sourcecols),sizeof(Uint16));
    load.read(reinterpret_cast<char*>(&spritespacer),sizeof(Uint8));
    load.read(reinterpret_cast<char*>(&spritewidthoffset),sizeof(Uint8));
    load.read(reinterpret_cast<char*>(&spriteheightoffset),sizeof(Uint8));
    load.read(reinterpret_cast<char*>(&datablocks),sizeof(Uint16));

    TRACE("Disk","Load > spritespacer: %d",spritespacer);
    TRACE("Disk","Load > spriteheightoffset: %d",spriteheightoffset);
    TRACE("Disk","Load > spritewidthoffset: %d",spritewidthoffset);

    // ------------ [ start setup level ] --------------------
    levellayer = new cSpriteLayer(levelrows, levelcols, spriteheight, spritewidth, false, true, true, 0, 0, 0);

    //Setup Source
    levellayer->source->spritespacer = spritespacer;
    levellayer->source->load(tilesource);
    levellayer->source->spritewidthoffset = spritewidthoffset;
    levellayer->source->spriteheightoffset = spriteheightoffset;
    levellayer->source->spriteheight = spriteheight;
    levellayer->source->spritewidth = spritewidth;

    // Get Slopes
    levellayer->source->getSlopes();

    // Read DataBlocks from file
    for (int sprite = 0; sprite < datablocks; sprite++) {
        Uint8 row;
        Uint8 col;
        Uint8 type;
        Uint8 sheetrow;
        Uint8 sheetindex;

        load.read(reinterpret_cast<char*>(&row),sizeof(Uint8));
        load.read(reinterpret_cast<char*>(&col),sizeof(Uint8));
        load.read(reinterpret_cast<char*>(&type),sizeof(Uint8));
        load.read(reinterpret_cast<char*>(&sheetrow),sizeof(Uint8));
        load.read(reinterpret_cast<char*>(&sheetindex),sizeof(Uint8));

        levellayer->leveldata[row][col].type=type;
        levellayer->leveldata[row][col].row=sheetrow;
        levellayer->leveldata[row][col].index=sheetindex;
    }

    levellayer->render(0,0); //Because this is a buffered layer, we render it once direct after construction of the data array.
	levellayer->createbuffer = false; //Enable buffered rendering

	/* Add levellayer to backgroundlayers list */
	backgrounds.push_back(levellayer);

    // ------------ [ start setup player ] --------------------

    char playerbitmap[]="player.bmp";
    playerobject = new cPlayer(playerbitmap, 40, 32);
    objects.push_back(playerobject);    //Add to level object list

    // Close File
    load.close();
	
}

bool cLevel::save(string file){
	
}
