#ifndef UUID_H
#define UUID_H

#include <iostream>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;


#include "text-helpers.h"

class PseudoUuid
{
public:
	PseudoUuid()
	{}

public:
	friend std::ostream& operator << (std::ostream &s, const PseudoUuid &id)
	{
		return s << id.Get();
	}

	void operator=(const string &s )
	{
		Set(s);
	}

	string Get() const
	{
		return m_uuid.empty() ? EmptyUuid : m_uuid;
	}

	void Set(const string &id)
	{
		if(ValidateId(id))
		{
			m_uuid = id;
		}	
	}

	static bool ValidateId(const string &id)
	{
		return id.length() == 32 ? IsHex(id) : false;
	}

	static string GenerateId()
	{
		std::stringstream ss;
		for(auto i = 0; i < 16; i++)
		{
			auto rc = RandomChar();
			stringstream hexstream;
			hexstream << std::hex << int(rc);
			auto hex = hexstream.str(); 
			ss << (hex.length() < 2 ? '0' + hex : hex);
		}        
		return ss.str();
	}
	
private:
	string m_uuid;

public:
	static inline string EmptyUuid = "00000000000000000000000000000000";
};


#endif
