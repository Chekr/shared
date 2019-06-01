#ifndef _TEXT_HELPERS_H_
#define _TEXT_HELPERS_H_

#include <random>
#include <algorithm>
#include <regex>
#include <string>
using std::string;
using std::regex;

inline unsigned char RandomChar()
{
	std::random_device rd;
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dis(0, 255);
	return static_cast<unsigned char>(dis(gen));
}

inline bool IsHex(string text)
{
	regex hexRegex("-?[0-9a-fA-F]+");

	return regex_match(text, hexRegex);
}


#endif
