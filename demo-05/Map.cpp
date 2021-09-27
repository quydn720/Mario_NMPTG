#include "Map.h"
#define MAX_CHAR_LINE 1024
#define INFO_SECTION 0
#define INFO_MAP 1

Map::Map(std::wstring path)
{
	Load(path);
}

void Map::Load(std::wstring path)
{
	std::ifstream f;
	f.open(path);

	int numTokens = 0;
	int section = -1;
	char str[MAX_CHAR_LINE];
	char* next_token = NULL;
	while (f.getline(str, MAX_CHAR_LINE)) {
		char* token = strtok_s(str, " ", &next_token);

		// Can add some settings, like tokens size at here, 
		// TODO: may be won't need the tokens size, cuz sprite has 5 tokens
		if (token == NULL) continue; // blank space
		if (token[0] == '/') continue; // comments
		if (token[0] == '#') {
			section = atoi(strtok_s(NULL, " ", &next_token));
			if (section == INFO_SECTION) {
				numTokens = atoi(strtok_s(NULL, " ", &next_token));
				continue;
			}
			else if (section == INFO_MAP) {
				int texId = atoi(strtok_s(NULL, " ", &next_token));
				tex = CTextures::GetInstance()->Get(texId);
				numTokens = atoi(strtok_s(NULL, " ", &next_token));
			}
			continue;
		}
		// Read the line and store into vector until end (null)
		std::vector<int> tokens;
		while (token != NULL) {
			tokens.push_back(atoi(token));
			token = strtok_s(NULL, " ", &next_token);
		}
		if (section == INFO_SECTION) {
			LoadInfoFromToken(tokens, numTokens);
		}
		else if (section == INFO_MAP) {
			LoadMapFromToken(tokens);
		}
	}
	LoadMapTiles();
	f.close();
	DebugOut(L"[MAP] Map loaded successfully!\n");
}

void Map::Render()
{
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			int id = tiles[i][j];
			float x = tileSize * j;
			float y = tileSize * i;
			CSprites::GetInstance()->Get(id)->Draw(x, y);
		}
	}
}

void Map::LoadInfoFromToken(vector<int> tokens, int numTokens) {
	if (tokens.size() == numTokens) {
		numRows = tokens[0];
		numColumns = tokens[1];
		numTileRows = tokens[2];
		numTileColumns = tokens[3];
		tileSize = tokens[4];
	}
}

void Map::LoadMapFromToken(vector<int> row) {
	for (int i = 0; i < row.size(); i++) {
		tiles[currentRow][i] = row[i] - 1;
	}
	currentRow++;
}

void Map::LoadMapTiles() {
	int id = 0;
	for (int i = 0; i < numTileRows; i++)
	{
		for (int j = 0; j < numTileColumns; j++)
		{
			RECT r;
			r.left = id % numTileColumns * tileSize;
			r.top = (id / numTileColumns) * tileSize;
			r.right = r.left + tileSize;
			r.bottom = r.top + tileSize;
			CSprites::GetInstance()->Add(id, r.left, r.top, r.right, r.bottom, tex);
			id++;
		}
	}
}