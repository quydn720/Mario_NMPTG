#pragma once
#include <Windows.h>
#include <unordered_map>

#include <d3dx10.h>
#include <fstream>   
#include "Texture.h"
#include "Sprite.h"
#include "Textures.h"


/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	std::unordered_map<int, LPSPRITE> sprites;

public:
	/*
	* Load all sprites from a file (formatted for this function)
	*/
	void LoadFromFile(std::wstring path);
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);

	static CSprites* GetInstance();
};

