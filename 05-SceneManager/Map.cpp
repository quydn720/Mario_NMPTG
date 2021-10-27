#include "Map.h"
#include "debug.h"
#include "Sprites.h"

#define MAP_INFO_SECTION 1
#define MAP_TILE_SECTION 2
#define MAP_UNKNOWN_SECTION -1
#define TEX_MAP_ID = 30
CMap::CMap(wstring path)
{
	Load(path);
}

void CMap::Load(wstring path)
{
	ifstream f;
	f.open(path);

	// current resource section flag
	int section = MAP_UNKNOWN_SECTION;
	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE)) {
		string line(str);
		if (line[0] == '#') continue;
		if (line == "[Info]") {
			section = MAP_INFO_SECTION;
			continue;
		}
		if (line == "[Map]") {
			section = MAP_TILE_SECTION;
			continue;
		}
		if (line[0] == '[') {
			section = MAP_UNKNOWN_SECTION;
			continue;
		}

		switch (section) {
		case MAP_INFO_SECTION:
			_ParseSection_Info(line);
			break;
		case MAP_TILE_SECTION:
			_ParseSection_MapTile(line);
			break;
		}
	}
	f.close();
	LoadMapTiles();
}

void CMap::Render()
{
	LPTEXTURE tex = CTextures::GetInstance()->Get(30);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int id = tiles[i][j];
			if (id != -1) {
				/*RECT r;
				r.left = id % tileColumn * tileSize;
				r.top = (id / tileColumn) * tileSize;
				r.right = r.left + tileSize;
				r.bottom = r.top + tileSize;*/

				/*float c_left, c_top;
				c->GetPostion(c_left, c_top);
				float c_width = c->GetWidth();
				float c_height = c->GetHeight();*/
				/*if (r.left > c_left + c_width || r.right < c_left || r.top > c_top + c_height || r.bottom < c_top) {
					continue;
				}*/
				float x = tileSize * j;
				float y = tileSize * i;
				CSprites::GetInstance()->Get(id)->Draw(x, y);
				/*CGame::GetInstance()->Draw((float)(x), (float)(y), tex, r.left, r.top, r.right, r.bottom);*/

			}
		}
	}
}

void CMap::_ParseSection_MapTile(string line)
{
	vector<string> tokens = split(line);

	size_t size = tokens.size();
	if (size < column) return;
	for (int i = 0; i < column; i++) {
		tiles[currentRow][i] = atoi(tokens[i].c_str()) - 1;
	}
	currentRow++;
}

void CMap::_ParseSection_Info(string line)
{
	vector<string> tokens = split(line);
	size_t size = tokens.size();

	if (size < 5) return;
	row = atoi(tokens[0].c_str());
	column = atoi(tokens[1].c_str());
	tileRow = atoi(tokens[2].c_str());
	tileColumn = atoi(tokens[3].c_str());
	tileSize = atoi(tokens[4].c_str());
	tex = CTextures::GetInstance()->Get(atoi(tokens[5].c_str()));
}

void CMap::LoadMapTiles() {
	int id = 0;
	for (int i = 0; i < tileRow; i++)
	{
		for (int j = 0; j < tileColumn; j++)
		{
			RECT r;
			r.left = id % tileColumn * tileSize;
			r.top = (id / tileColumn) * tileSize;
			r.right = r.left + tileSize;
			r.bottom = r.top + tileSize;
			CSprites::GetInstance()->Add(id, r.left, r.top, r.right, r.bottom, tex);
			id++;
		}
	}
}

CMap::~CMap()
{
}
