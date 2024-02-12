#include "file-helper.h"

#include "log.h"

#include <experimental/filesystem> // C++-standard header file name  

#ifdef WIN32
#include <filesystem> // Microsoft-specific implementation header file name  
#endif

//using namespace std::experimental::filesystem::v1;
using namespace std::experimental::filesystem;

namespace FileHelper
{
	bool DoesDirectoryExist(std::string directory)
	{
		return std::experimental::filesystem::exists(directory);
	}

	bool DoesFileExist(std::string filename)
	{
		return std::experimental::filesystem::exists(filename);
	}

	bool MakeDirectory(std::string directory)
	{
		return std::experimental::filesystem::create_directories(directory);
	}

	void WriteFile(std::string filename, std::vector<std::string> outLines)
	{
		ofstream myfile;
		myfile.open(filename);
		for (std::string outputLine : outLines)
		{
			myfile << outputLine << endl;
		}

		myfile.close();
	}

	void AppendFile(std::string filename, std::vector<std::string> outLines)
	{
		ofstream myfile;
		myfile.open(filename, std::ios_base::app);
		for (std::string outputLine : outLines)
		{
			myfile << outputLine << endl;
		}

		myfile.close();
	}

	void AppendFile(std::string filename, std::string outputLine)
	{
		ofstream myfile;
		myfile.open(filename, std::ios_base::app);

		myfile << outputLine << endl;

		myfile.close();
	}

	std::vector<std::string> ReadFile(std::string filename)
	{
		std::vector<std::string> output;


		std::string line;
		ifstream inputFile(filename);
		if (inputFile.is_open())
		{
			if (getline(inputFile, line))
			{
				std::string first_three = line.substr(0, 3);
				const std::string test = "\xEF\xBB\xBF";
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
