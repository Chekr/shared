#ifndef _TIME_HELPER_H_
#define _TIME_HELPER_H_

#include <string>
#include <sstream>


#include <chrono>

/*
std::string FormatSeconds(double seconds)
{
	int days = input_seconds / 60 / 60 / 24;
	int hours = (input_seconds / 60 / 60) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
}

std::string FormatSeconds(double seconds)
{
	int d = input_seconds / 60 / 60 / 24;
	int h = (input_seconds / 60 / 60) % 24;
	int m = (input_seconds / 60) % 60;
	int s = input_seconds % 60;i


	return d << ":" << h << ":" << m << ":" << s;
}


std::string TimeToString(std::chrono::time_point time)
{
	char buff[100];
	auto t = std::chrono::system_clock::to_time_t(time);
	struct tm tm;
	return std::gmtime_r(&t, &tm) &&
		std::strftime(buff, sizeof(buff), "%F", &tm)
		? std::string{ buff } : std::string;	
}
*/

std::string CurrentDate(bool includeDayOfWeek = false)
{
	time_t now = time(NULL);
	struct tm tstruct;
	char buf[40];
	tstruct = *localtime(&now);
	std::string formatter = includeDayOfWeek ? "%A %Y-%m-%d" : "%Y-%m-%d";
	strftime(buf, sizeof(buf), formatter.c_str(), &tstruct);

	return buf;
}

std::string CurrentTime()
{
	time_t now = time(NULL);
	struct tm tstruct;
	char buf[40];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}

#endif
