#include "file-helper.h"

#include "log.h"

#include <experimental/filesystem> // C++-standard header file name  

#ifdef WIN32
#include <filesystem> // Microsoft-specific implementation header file name  
#endif

using namespace std::experimental::filesystem::v1;

namespace FileHelper
{
	bool DoesDirectoryExist(string directory)
	{
		return std::experimental::filesystem::exists(directory);
	}

	bool DoesFileExist(string filename)
	{
		return std::experimental::filesystem::exists(filename);
	}

	bool MakeDirectory(string directory)
	{
		return std::experimental::filesystem::create_directories(directory);
	}

	void WriteFile(string filename, vector<string> outLines)
	{
		ofstream myfile;
		myfile.open(filename);
		for (string outputLine : outLines)
		{
			myfile << outputLine << endl;
		}

		myfile.close();
	}

	void AppendFile(string filename, vector<string> outLines)
	{
		ofstream myfile;
		myfile.open(filename, std::ios_base::app);
		for (string outputLine : outLines)
		{
			myfile << outputLine << endl;
		}

		myfile.close();
	}

	void AppendFile(string filename, string outputLine)
	{
		ofstream myfile;
		myfile.open(filename, std::ios_base::app);

		myfile << outputLine << endl;

		myfile.close();
	}

	vector<string> ReadFile(string filename)
	{
		vector<string> output;

		string line;
		ifstream inputFile(filename);
		if (inputFile.is_open())
		{
			if (getline(inputFile, line))
			{
				string first_three = line.substr(0, 3);
				const string test = "\xEF\xBB\xBF";
				if (first_three == test)
				{
					line = line.substr(3);
				}

				output.push_back(line);
			}
			while (getline(inputFile, line))
			{
				output.push_back(line);
			}
			inputFile.close();
		}
		else
		{
			Logger::Log("could not load file: " + filename);
		}

		return output;
	}

}
