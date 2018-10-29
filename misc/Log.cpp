#include "Log.h"
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <sstream>

#include "FileHelper.h"

using std::chrono::system_clock;

namespace Logger
{
	void Log(string message)
	{
		system_clock::time_point now = system_clock::now();
		time_t tt = system_clock::to_time_t(now);
		tm utc_tm = *gmtime(&tt);
		std::stringstream ss;
		ss << (utc_tm.tm_year + 1900) << "-" << (utc_tm.tm_mon + 1) << "-" << utc_tm.tm_mday;
		string date = ss.str();



		FileHelper::AppendFile("./log/" + date + "-Log.txt", message);
	}
}
