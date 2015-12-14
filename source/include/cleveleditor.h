#ifndef CLEVELEDITOR_H
#define CLEVELEDITOR_H


#include <string>

class cLevelEditor
{
	private:
		void init();
		void drawSpritePicker();
		void drawSpritePickerPanel();
		void drawSpritePickerScrollBar();
		void drawSpritePickerTiles();
		void drawSpriteSelector();
		void loadPickerSource(string filename);
		void drawRectangle(int left, int top, int width, int height, int r, int g, int b, int a);
		
		void render2();
		int getTileCol(int x, int tilewidth);
		int getTileRow(int y, int tileheight);
		bool showspritepalet;
		cSprite* pickersource;
		int pickercols;
		int tilewidth;
		int tileheight;
		int scrollbarwidth;
		int rowsinsource;
		int colsinsource;
		
	public:
		cLevelEditor();
		~cLevelEditor();
		void render();
	protected:
};

#endif
