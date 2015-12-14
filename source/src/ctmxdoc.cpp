#include "../include/ctmxdoc.h"
#include "../include/cdebug.h"
#include "../include/cbase64.h"
#include "../include/cdecompress.h"



cTMXDoc::cTMXDoc()
{
	TRACE("Construct","cTMXDoc()");
	init();
}

cTMXDoc::~cTMXDoc()
{
}

bool cTMXDoc::VisitEnter ( const TiXmlElement &elem, const TiXmlAttribute *attrib )
{
    if ( string( "map" ) == elem.Value() )
    {
        elem.Attribute( "width", &m_NumMapColumns );
        elem.Attribute( "height", &m_NumMapRows );

        printf( "Loading TileMap : %ix%i\n", m_NumMapColumns*32, m_NumMapRows*32 );
    }
    else if ( string( "tileset" ) == elem.Value() )
    {
        elem.Attribute( "tilewidth", &m_TileWidth );
        elem.Attribute( "tileheight", &m_TileHeight );
        elem.Attribute( "spacing", &m_TileSpacing );
        elem.Attribute( "margin", &m_TilesetMargin );

        printf( "Tiles : %ix%i\n", getTileWidth(), getTileHeight() );
    }
    else if ( string( "image" ) == elem.Value() )
    {

    }
    else if ( string( "tile" ) == elem.Value() )
    {
        //printf( "%s\n", elem.Attribute( "id" ) );
        //printf( "%s\n", elem.FirstChildElement()->FirstChildElement()->Attribute( "name" ) );
    }

    else if ( string( "layer" ) == elem.Value() )
    {
        // We don't neet to use layer width and height yet.
        //elem.Attribute("name");
        //elem.Attribute("width");
        //elem.Attribute("height");
    }
    else if ( string( "data" ) == elem.Value() )
    {
        const char* text = elem.GetText();
        decode_and_store_map_data( text );
        buildMapImage();
    }
    return true;
}

void cTMXDoc::decode_and_store_map_data( string encoded_data )
{
    cBase64 b;
    cDecompress dec;

    const string mDecoded = b.base64_decode(encoded_data);
    const int mSize = ( m_NumMapColumns * m_NumMapRows ) * 4;
    const unsigned char *data = reinterpret_cast<const unsigned char*>( dec.decompress( mDecoded, mSize ) );

    vector< int > layerDataRow( getNumMapColumns() );
    int m_LayerRow = 0;
    int m_LayerCol = 0;

    for (int i = 0; i < getNumMapRows(); i++)
    {
        m_LayerData.push_back( layerDataRow );
    }

    for( int i = 0; i < mSize - 3; i += 4 )
    {
        const int gid = data[i] | data[i + 1] << 8 | data[i + 2] << 16 | data[i + 3] << 24;

        m_LayerData[m_LayerRow][m_LayerCol] = gid;

        m_LayerCol++;
        if ( m_LayerCol == getNumMapColumns()  )
        {
            m_LayerCol = 0;
            m_LayerRow++;
        }
    }
}

void cTMXDoc::init(){
	doc = NULL;
}

bool cTMXDoc::load(){
    doc = new TiXmlDocument("Data\\test.tmx");

    if (!doc->LoadFile())
    {
    	TRACE("TMXDoc","XML Document load failed.");
        return false;
    }
    doc->Accept(this);
	TRACE("TMXDoc","XML Document loaded.");
    return true;
}

void cTMXDoc::buildMapImage(){
	
}
