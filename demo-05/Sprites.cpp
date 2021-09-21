#include "Sprites.h"
#include "Game.h"
#include "debug.h"

// Store the line from file
#define MAX_CHAR_LINE 1024

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::LoadFromFile(std::wstring path)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", path);
	LPTEXTURE tex = NULL;
	int numTokens = 0;

	std::ifstream f;
	f.open(path);
	char str[MAX_CHAR_LINE];
	char* next_token = NULL;
	while (f.getline(str, MAX_CHAR_LINE)) {
		char* token = strtok_s(str, " ", &next_token);

		// Can add some settings, like tokens size at here, 
		// TODO: may be won't need the tokens size, cuz sprite has 5 tokens
		if (token == NULL) continue; // blank space
		if (token[0] == '/') continue; // comments
		if (token[0] == '#') {
			int texId = atoi(strtok_s(NULL, " ", &next_token));
			tex = CTextures::GetInstance()->Get(texId);
			numTokens = atoi(strtok_s(NULL, " ", &next_token));
			continue;
		}
		// Add the sprite to database
		std::vector<int> tokens;
		while (token != NULL) {
			tokens.push_back(atoi(token));
			token = strtok_s(NULL, " ", &next_token);
		}
		if (tokens.size() == numTokens) {
			CSprites::GetInstance()->Add(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tex);
		}
	}
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}