/*
				Title		:   Windows Console Utility

				Name		:
				Author		:	Kadda Aoues
				Date		:
				Statu		:	On Development
				Version		:
				Copyright	:

				Discription :
				
				This utility for Windows, MacOS and Linux envirenment



				COLORING CHAR PARAMETER :
				FOREGROUND_BLUE				0x0001 // text color contains blue.
				FOREGROUND_GREEN			0x0002 // text color contains green.
				FOREGROUND_RED				0x0004 // text color contains red.
				FOREGROUND_INTENSITY		0x0008 // text color is intensified.
				BACKGROUND_BLUE				0x0010 // background color contains blue.
				BACKGROUND_GREEN			0x0020 // background color contains green.
				BACKGROUND_RED				0x0040 // background color contains red.
				BACKGROUND_INTENSITY		0x0080 // background color is intensified.
				COMMON_LVB_LEADING_BYTE		0x0100 // Leading Byte of DBCS
				COMMON_LVB_TRAILING_BYTE	0x0200 // Trailing Byte of DBCS
				COMMON_LVB_GRID_HORIZONTAL	0x0400 // DBCS: Grid attribute: top horizontal.
				COMMON_LVB_GRID_LVERTICAL	0x0800 // DBCS: Grid attribute: left vertical.
				COMMON_LVB_GRID_RVERTICAL	0x1000 // DBCS: Grid attribute: right vertical.
				COMMON_LVB_REVERSE_VIDEO	0x4000 // DBCS: Reverse fore/back ground attribute.
				COMMON_LVB_UNDERSCORE		0x8000 // DBCS: Underscore.
					
				COMMON_LVB_SBCSDBCS			0x0300 // SBCS or DBCS flag.

*/

#pragma once
#include <utility>
#include <stdexcept>
#include <iostream>

#if defined(_WIN32)
#include <Windows.h>
#elif	defined(__APPLE__)
#include <CoreGraphics/CoreGraphics.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif



#define VALID_RECT(rect)		((rect##.top != 0) && (rect##.left != 0) && (rect##.right != 0) && (rect##.bottom != 0))
		
#define KeyPressed(Key)            (GetAsyncKeyState(Key) & 0x8000)
#define KeyReleased(Key)           (GetAsyncKeyState(Key) & 0x0001)
#define KeyOn(Key)                 (GetAsyncKeyState(Key) & 0x8001)

#define FocusKeyPressed(Key)            (__IsFocus && (GetAsyncKeyState(Key) & 0x8000))
#define FocusKeyReleased(Key)           (__IsFocus && (GetAsyncKeyState(Key) & 0x0001))
#define FocusKeyOn(Key)                 (__IsFocus && (GetAsyncKeyState(Key) & 0x8001))

#define  CTRL_KEY_PRESSED(key)		(((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0) &&	\
									((GetAsyncKeyState(key) & 0x8000) != 0))
#define  CTRL_KEY_RELEASED(key)		((GetAsyncKeyState(key) & 0x0001) != 0) &&			\
									((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0))

#define  FOCUS_CTRL_KEY_PRESSED(key)	(__IsFocus &&((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0) &&	\
										((GetAsyncKeyState(key) & 0x8000) != 0))
#define  FOCUS_CTRL_KEY_RELEASED(key)	(__IsFocus &&((GetAsyncKeyState(key) & 0x0001) != 0) &&			\
										((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0))

#define ENUM_CTRL_KEY(key)		_CONSOLE_KeyEvent_##key
#define DEF_ENUM_CTRL_KEY(key)    enum _CONSOLE_KeyEvent_##key { \
_CONSOLE_NONE_##key, \
_CONSOLE_KEY_##key,\
_CONSOLE_CTRL_KEY_##key\
}

// definition of enum for key and control + key event detection
DEF_ENUM_CTRL_KEY();


namespace console {
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			PRESS K AND CTRL + KEY/ THIS FUNCTION MAKE DIFFERENTCE BETWEEN KEY AND CTRL+KEY
// 			USING WITH SWITCH 
//  '''
//	auto ev_K = PressKey(L'K');
//	switch (ev_K) {
//	case _KEY_:print_ << "Pressed K " << ++countK << end_; break;
//	case _CTRL_KEY_:print_ << "Pressed Ctrl+K " << ++countK << end_; break;
//	}
//	'''
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	ENUM_CTRL_KEY() PressKey(short key, bool isFocus = true) {
		if (isFocus)
			if (KeyReleased(key)) {
				if (KeyPressed(VK_CONTROL))
					return _CONSOLE_CTRL_KEY_;
				else
					return _CONSOLE_KEY_;
			}

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		GOTO FUNCTION CONSOLE INDIPENDENT
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void gotoxy(HANDLE console, int col, int raw) {
		COORD pos;
		pos.X = col;
		pos.Y = raw;

		SetConsoleCursorPosition(console, pos);
	}

	void gotoxy(int col, int raw) {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console == INVALID_HANDLE_VALUE) return;

		gotoxy(console, col, raw);
	}

	void gotoxy(HANDLE console, COORD pos) {
		gotoxy(console, pos.X, pos.Y);
	}

	void gotoxy(COORD pos) {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console == INVALID_HANDLE_VALUE) return;
		gotoxy(console, pos.X, pos.Y);
	}

	COORD get_cursor_position(HANDLE console) {
		COORD invalid = { -1,-1 };

		if (console == INVALID_HANDLE_VALUE) return invalid;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(console, &csbi)) return invalid;

		return csbi.dwCursorPosition;
	}

	COORD get_cursor_position() {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		return get_cursor_position(console);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		CLEAR FUNCTION LINE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void clear_line(HANDLE console, int line) {

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(console, &csbi)) return;

		DWORD	Width = csbi.dwSize.X;
		COORD	coordLine = { 0, (SHORT)line };

		DWORD	writen{};

		// Fill the whole line with spaces
		FillConsoleOutputCharacter(console, L' ', Width, coordLine, &writen);

		// Reset attributes colors for the line
		FillConsoleOutputAttribute(console, csbi.wAttributes, Width, coordLine, &writen);

		// Restor cursor to 0, line.
		gotoxy(0, line);
	}

	void clear_line(int line) {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console == INVALID_HANDLE_VALUE) return;
		clear_line(console, line);
	}

	void clear_line(int from, int to) {
		for (int i = from; i <= to; ++i) clear_line(i);
	}

	void clear_line(HANDLE console, int from, int to) {
		for (int i = from; i < to + 1; ++i)
			clear_line(console, i);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PRINT FUNCTION AT LINE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void print_at(HANDLE console, const std::string& comment, int x, int y) {
		clear_line(console, y);
		gotoxy(console, x, y);
		std::cout << comment;
	}

	void print_at(const std::string& comment, int x, int y) {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		print_at(console, comment, x, y);
	}

	void print_color(HANDLE console, const std::string& text, WORD colors) {
		if (console == INVALID_HANDLE_VALUE) {
			std::cout << text;
			return;
		}

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(console, &csbi)) {
			std::cout << text;
			return;
		}

		//save current attributes
		WORD saveAttr = csbi.wAttributes;
		
		//set new attributes
		SetConsoleTextAttribute(console, colors);

		std::cout << text;

		// restor attributes
		SetConsoleTextAttribute(console, saveAttr);
	}

	void print_color(const std::string& text, WORD colors) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		print_color(console, text, colors);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			SEND EVENT KEY BOARD TO INPUT CONSOLE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool send_keyPressed(HANDLE console, short vk, bool keyDown = true) {
		INPUT_RECORD	record;
		DWORD			written;

		record.EventType = KEY_EVENT;
		record.Event.KeyEvent.bKeyDown = keyDown;
		record.Event.KeyEvent.wRepeatCount = 1;
		record.Event.KeyEvent.wVirtualKeyCode = vk;
		record.Event.KeyEvent.wVirtualScanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
		record.Event.KeyEvent.uChar.UnicodeChar = 0;
		record.Event.KeyEvent.dwControlKeyState = 0;

		return WriteConsoleInput(console, &record, 1, &written);
	}

	bool send_keyPressed(short vk, bool keyDown = true) {
		HANDLE  console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console == INVALID_HANDLE_VALUE) return false;
		return send_keyPressed(console, vk, keyDown);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		IS APPLICTION FOCUSED
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool is_focused(HWND WindConsole) {
		HWND hwnd = GetForegroundWindow();
		return WindConsole == hwnd;
	}

	bool is_focused() {
		HWND myHwnd = GetConsoleWindow();
		return is_focused(myHwnd);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			FUNCTION RETURN TRUE IF THERE EVENT SYSTEM - ANY - FALSE OTHER WISE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool pull_event(HANDLE& hConsoleIn) {
		//HANDLE  hConsoleIn = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsoleIn == INVALID_HANDLE_VALUE) return false;

		//INPUT_RECORD irb[32];
		DWORD events{};

		GetNumberOfConsoleInputEvents(hConsoleIn, &events);

		if (events > 0) {
			//ReadConsoleInput(hConsoleIn, irb, events, &events);
			return true;
		}
		else {
			return false;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			HIDE AND SHOW CURSOR
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void set_cursor_visibility(char visible) {
		HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO structCursorInfo;
		GetConsoleCursorInfo(hConsoleOutput, &structCursorInfo); // Get current cursor size
		structCursorInfo.bVisible = (visible ? TRUE : FALSE);
		SetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
	}
	
	void show_cursor() {
		set_cursor_visibility(1);
	}

	void hide_cursor() {
		set_cursor_visibility(0);
	}

	struct CursorHider {
		CursorHider()
		{
			hide_cursor();
		}
		~CursorHider()
		{
			show_cursor();
		}
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		CLEAN SCREEN AND PLACE CURSOR TO (0, 0)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void cls(HANDLE console) {

	}

	void cls() {
		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		const COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(hConsole, &csbi);
		const DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);

		GetConsoleScreenBufferInfo(hConsole, &csbi);
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

		SetConsoleCursorPosition(hConsole, coordScreen);
	}
}

