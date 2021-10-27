#include <string>
#include <fstream>
#include "Utils.h"
#include "Game.h"
#include "Textures.h"

#define MAX_MAP_LINE			1024

class CMap {
	int column, row = 0;
	int tileSize = 0;
	int tileColumn, tileRow = 0;
	int tiles[30][200] = { -1, -1 };
	int currentRow = 0; // util for parsing from text.
	LPTEXTURE tex = NULL;

public:
	CMap(wstring path);
	~CMap();

	int getMapWidth() { return column * tileSize; }
	int getMapHeight() { return row * tileSize; }
	void Load(wstring path);
	void Render();
	void _ParseSection_MapTile(string line);
	void _ParseSection_Info(string line);
	void LoadMapTiles();
};