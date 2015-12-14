#ifndef CTMXDOC_H
#define CTMXDOC_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tinyxml.h>
#include <Box2D\Box2D.h>

using namespace std;

class cTMXDoc : public TiXmlVisitor
{
	private:
		TiXmlDocument *doc;
		void init();
		int m_TileWidth;
        int m_TileHeight;
        int m_TileSpacing;
        int m_TilesetMargin;
        int m_NumMapColumns;
        int m_NumMapRows;
        ofstream myfile;
        vector< vector< int > > m_LayerData;
        void decode_and_store_map_data( string encoded_data );
        void buildMapImage();
	public:
		cTMXDoc();
		~cTMXDoc();
		bool load();
		virtual bool VisitEnter  (const TiXmlDocument  &);
        virtual bool VisitExit (const TiXmlDocument &);
        virtual bool VisitEnter (const TiXmlElement &, const TiXmlAttribute *);
        virtual bool VisitExit (const TiXmlElement &);
        virtual bool Visit (const TiXmlDeclaration &);
        virtual bool Visit (const TiXmlText &);
        virtual bool Visit (const TiXmlComment &);
        virtual bool Visit (const TiXmlUnknown &);
        
        int getTileWidth() { return m_TileWidth; }
        int getTileHeight() { return m_TileWidth; }
        int getTileSpacing() { return m_TileSpacing; }
        int getTilesetMargin() { return m_TilesetMargin; }
        int getNumMapColumns() { return m_NumMapColumns; }
        int getNumMapRows() { return m_NumMapRows; }
        
	protected:
};

#endif
