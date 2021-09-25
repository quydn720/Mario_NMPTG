#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include "debug.h"


class Map
{
private:
	int numRows, numColumns;
	int numTileRows, numTileColumns;
	int tileSize;
	int tiles[30][200] = { -1, -1 };
	int currentRow = 0;
	void LoadInfoFromToken(vector<int> tokens, int numTokens);
	void LoadMapFromToken(vector<int> row);
public:
	Map(std::wstring path);
	~Map();
	int getMapHeight() { return numColumns * tileSize; }
	int getMapWidth() { return numRows * tileSize; }
	void Load(std::wstring path);
	void Render();
};

