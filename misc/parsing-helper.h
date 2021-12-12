#ifndef PARSING_HELPER_H
#pragma once
#define PARSING_HELPER_H

#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <functional>
#include <cmath>
#include <iomanip>
#include <fstream>

using std::strspn;
using std::strcspn;
using std::string;


//enum StringType
//{
//	STRING = 0,
//	INT,
//	FLOAT
//};
//
//struct TypePair
//{
//public:
//	TypePair(string str, StringType type)
//	{
//		this->str = str;
//		this->type = type;
//	}
//public:
//	string str;
//	StringType type;
//};

static string ToUpperCase(const std::string& s)
{
	string output;
	transform(s.begin(), s.end(), std::back_inserter(output), toupper);
	return output;
}

static bool IsSpace(const char c)
{
	return (c == ' ') || (c == '\t');
}

static std::string ParseString(const char *&token)
{
	std::string s;
	token += strspn(token, " \t");
	size_t e = strcspn(token, " \t\r");
	s = std::string(token, &token[e]);
	token += e;
	return s;
}

static int ParseInt(const char *&token)
{
	token += strspn(token, " \t");
	int i = atoi(token);
	token += strcspn(token, " \t\r");
	return i;
}

static void ParseInt2(int &x, int &y, const char *&token)
{
	x = ParseInt(token);
	y = ParseInt(token);
}


static bool TryParseDouble(const char *s, const char *s_end, double *result)
{
	if (s >= s_end)
	{
		return false;
	}

	double mantissa = 0.0;
	// This exponent is base 2 rather than 10.
	// However the exponent we parse is supposed to be one of ten,
	// thus we must take care to convert the exponent/and or the 
	// mantissa to a * 2^E, where a is the mantissa and E is the
	// exponent.
	// To get the final double we will use ldexp, it requires the
	// exponent to be in base 2.
	int exponent = 0;

	// NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
	// TO JUMP OVER DEFINITIONS.
	char sign = '+';
	char exp_sign = '+';
	char const *curr = s;

	// How many characters were read in a loop. 
	int read = 0;
	// Tells whether a loop terminated due to reaching s_end.
	bool end_not_reached = false;

	/*
	BEGIN PARSING.
	*/

	// Find out what sign we've got.
	if (*curr == '+' || *curr == '-')
	{
		sign = *curr;
		curr++;
	}
	else if (isdigit(*curr)) { /* Pass through. */ }
	else
	{
		goto fail;
	}

	// Read the integer part.
	while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
	{
		mantissa *= 10;
		mantissa += static_cast<int>(*curr - 0x30);
		curr++;	read++;
	}

	// We must make sure we actually got something.
	if (read == 0)
		goto fail;
	// We allow numbers of form "#", "###" etc.
	if (!end_not_reached)
		goto assemble;

	// Read the decimal part.
	if (*curr == '.')
	{
		curr++;
		read = 1;
		while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
		{
			// NOTE: Don't use powf here, it will absolutely murder precision.
			mantissa += static_cast<int>(*curr - 0x30) * std::pow(10.0, -read);
			read++; curr++;
		}
	}
	else if (*curr == 'e' || *curr == 'E') {}
	else
	{
		goto assemble;
	}

	if (!end_not_reached)
		goto assemble;

	// Read the exponent part.
	if (*curr == 'e' || *curr == 'E')
	{
		curr++;
		// Figure out if a sign is present and if it is.
		if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-'))
		{
			exp_sign = *curr;
			curr++;
		}
		else if (isdigit(*curr)) { /* Pass through. */ }
		else
		{
			// Empty E is not allowed.
			goto fail;
		}

		read = 0;
		while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
		{
			exponent *= 10;
			exponent += static_cast<int>(*curr - 0x30);
			curr++;	read++;
		}
		exponent *= (exp_sign == '+' ? 1 : -1);
		if (read == 0)
			goto fail;
	}

assemble:
	*result = (sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
	return true;
fail:
	return false;
}

static float ParseFloat(const char *&token)
{
	token += strspn(token, " \t");
	const char *end = token + strcspn(token, " \t\r");
	double val = 0.0;
	TryParseDouble(token, end, &val);
	float f = static_cast<float>(val);
	token = end;

	return f;
}

static void ParseFloat2(float &x, float &y, const char *&token)
{
	x = ParseFloat(token);
	y = ParseFloat(token);
}

static string& LTrim(string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::ptr_fun<int, int>(std::isgraph)));
	return s;
}

static string& RTrim(string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::ptr_fun<int, int>(std::isgraph)).base(), s.end());
	return s;
}

//static string& trim(string& s)
//{
//	return ltrim(rtrim(s));
//}

static string Trim(const std::string& str,
	const std::string& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

static string RemoveBreaks(const std::string& str)
{
	string strResult = str;
	strResult.erase(std::remove(strResult.begin(), strResult.end(), '\n'), strResult.end());
	return strResult;
}

static bool IsInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

static bool IsTrue(const std::string& s)
{
	if(s == "") { return false; }

	string stringBool = ToUpperCase(s);
	if(stringBool == "Y") { return true; }
	if(stringBool == "YES") { return true; }
	if(stringBool == "T") { return true; }
	if(stringBool == "TRUE") { return true; }

	if(IsInteger(stringBool))
	{
		return stoi(stringBool) != 0;
	}
	return false;

}



static std::string BoolToString(const bool& value)
{
	return value ? "true" : "false";
}

template <class T>
inline std::string TypeToString(const T& t, int precision = 2)
{
	std::ostringstream buffer;

	buffer << std::fixed << std::setprecision(precision) << t;

	return buffer.str();
}

template <typename T>
inline T GetValueFromStream(std::ifstream& stream)
{
  T val;
  stream >> val;

  if (!stream.is_open())
  {
    throw std::runtime_error("Invalid type parsed from stream.");
  }

  return val;
}


#endif
