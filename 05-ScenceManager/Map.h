#include <string>
#include <fstream>
#include "Utils.h"
#include "Game.h"
#include "Textures.h"

#define MAX_MAP_LINE			1024
#define MAP_INFO				1
#define MAP_SECTION_ROWS		2
#define MAP_SECTION_UNKNOWN		-1

class Map {
	static Map* __instance;

	int column, row;
	int tileSize;
	int tileColumn, tileRow;
	int tileId[200][200] = { 0, 0 };
	int currentRow; // util for parsing from text.
public:
	Map();
	~Map();

	static Map* GetInstance();
	void Load(wstring path);
	void Render();
	void _ParseSection_ROWS(string line);
	void _ParseSection_INFO(string line);
};