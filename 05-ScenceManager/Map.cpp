#include "Map.h"

Map::Map()
{
	currentRow = column = row = tileSize = tileColumn = tileRow = 0;
}

void Map::Load(wstring path)
{
	ifstream f;
	f.open(path);

	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;
	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE)) {
		string line(str);
		if (line[0] == '#') continue;
		if (line == "[INFO]") {
			section = MAP_INFO;
			continue;
		}
		if (line == "[ROWS]") {
			section = MAP_SECTION_ROWS;
			continue;
		}
		if (line[0] == '[') {
			section = MAP_SECTION_UNKNOWN;
			continue;
		}


		switch (section) {
		case MAP_INFO:
			_ParseSection_INFO(line);
			break;
		case MAP_SECTION_ROWS:
			_ParseSection_ROWS(line);
			break;
		}
	}
	f.close();
	DebugOut(L"Map loaded successfully");
}

void Map::Render()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int id = tileId[i][j];
			RECT r = {
			   id % tileColumn * tileSize,								// left
			   (id / tileColumn) * tileSize,							// top
			   id % tileColumn * tileSize + tileSize,					// right
			   (id / tileColumn) * tileSize + tileSize 					// bottom
			};

			// Draw -- HARD-CODE Map texture ID, need to put it inside file txt.
			CGame::GetInstance()->Draw(j * tileSize, i * tileSize, CTextures::GetInstance()->Get(30), r);
		}
	}
}

void Map::_ParseSection_ROWS(string line)
{
	vector<string> tokens = split(line);

	int size = tokens.size();
	if (size < column) return;
	for (int i = 0; i < column; i++) {
		int id = atoi(tokens[i].c_str()) - 1;	// because when using Tiled to export to json file, the id auto increase 1 unit
		tileId[currentRow][i] = id;
	}
	currentRow++;
}

void Map::_ParseSection_INFO(string line)
{
	vector<string> tokens = split(line);
	int size = tokens.size();

	if (size < 5) return;
	row = atoi(tokens[0].c_str());
	column = atoi(tokens[1].c_str());
	tileRow = atoi(tokens[2].c_str());
	tileColumn = atoi(tokens[3].c_str());
	tileSize = atoi(tokens[4].c_str());
}

Map::~Map()
{
}
