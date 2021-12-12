#ifndef CMD_HELPER_H
#define CMD_HELPER_H

#include <string>
#include <algorithm>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;


#include "parsing-helper.h"

#include <thread>
#include <chrono>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <term.h>
#endif

// http://www.cplusplus.com/forum/articles/10515/
// Michael Thomas Greer - http://www.cplusplus.com/user/Duthomhas/
static void ClearScreen()
{
#ifdef _WIN32
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	// Get the number of cells in the input buffer.
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) { return; }

	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	// Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(
				hStdOut,
				(TCHAR) ' ',
				cellCount,
				homeCoords,
				&count
				)) return;

	// Fill the buffer using the current settings
	if (!FillConsoleOutputAttribute(
				hStdOut,
				csbi.wAttributes,
				cellCount,
				homeCoords,
				&count
				)) return;

	// Move the cursor to the Home position
	SetConsoleCursorPosition( hStdOut, homeCoords );
#else
	if (!cur_term)
	{
		int result;
		setupterm( NULL, STDOUT_FILENO, &result );
		if (result <= 0) return;
	}

	putp( tigetstr( "clear" ) );
#endif

}

static bool ConfirmOption()
{
	string line;
	std::getline(cin, line);
	line = ToUpperCase(Trim(line));
	while(true)
	{
		if(line == "Y")
		{
			return true;
		}
		if(line == "N")
		{
			return false;
		}
		cout << "Invalid selection (y/n): ";
		std::getline(cin, line);
		line = ToUpperCase(Trim(line));
	}
}

static void SafeSleep(long milliseconds)
{

	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


#endif
