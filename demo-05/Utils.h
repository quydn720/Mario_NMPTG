#pragma once
#include <string>
#include <vector>
#include <fstream>
#define MAX_CHAR_LINE 1024

class Utils
{
public:
	static std::vector<int> from(std::wstring path, int numTokens) {
		std::ifstream f;
		f.open(path);
		char str[MAX_CHAR_LINE];
		char* next_token = NULL;
		while (f.getline(str, MAX_CHAR_LINE)) {
			char* token = strtok_s(str, " ", &next_token);
			std::vector<int> tokens;

			while (token != NULL) {
				if (token != NULL) {
					tokens.push_back(atoi(token));
				}
				token = strtok_s(NULL, " ", &next_token);
			}
			if (tokens.size() == numTokens) {
				return tokens;
			}
		}
	}
};

