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

void CSprites::AddFromFile(std::wstring path)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", path);
	
	LPTEXTURE texMario = CTextures::GetInstance()->Get(0);
	
	std::ifstream f;
	f.open(path);
	char str[MAX_CHAR_LINE];
	char* next_token = NULL;
	while (f.getline(str, MAX_CHAR_LINE)) {
		char* token = strtok_s(str, " ", &next_token);
		std::vector<int> tokens;

		while (token != NULL) {
			DebugOut(L"%d\n", atoi(token));
			if (token != NULL) {
				tokens.push_back(atoi(token));
			}
			token = strtok_s(NULL, " ", &next_token);
		}
		if (tokens.size() == 5) {
			CSprites::GetInstance()->Add(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], texMario);
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