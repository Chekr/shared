#ifndef _FILE_HELPER_H_
#pragma once
#define _FILE_HELPER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::string;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::endl;

namespace FileHelper
{
	bool DoesDirectoryExist(string directory);
	bool DoesFileExist(string filename);
	bool MakeDirectory(string directory);
	void WriteFile(string filename, vector<string> outLines);
	void AppendFile(string filename, vector<string> outLines);
	void AppendFile(string filename, string outLine);
	vector<string> ReadFile(string filename);
}

#endif