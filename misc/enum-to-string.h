#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H

#include <string>

class EnumToString
{
public:
	virtual std::string Convert(int enumeration)=0;
};

#endif
