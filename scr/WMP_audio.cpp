/*
				Title

				Name		:
				Author		:	Kadda Aoues
				Date		:
				Statu		:	On Development
				Version		:
				Copyright	:

				Discription :
				This program is made in profite to test diffirente algorithm and functions
				of selection of object and groupe selection

				DONE : 1. Play Current file audio
				DONE : 2. delete  -  clear list
				DONE : 7. Custom delete, insert -- only audio files --
				DONE : 4. Load from begin
				DONE : 3. Auto play Need Some Perfection 
				DONE : 0. change standard map control to key that nex to space "zxcvbn
				DONE : 6. Make key map for key_play of application


				TODO : 5. Make config file for application config.json, and save.json playlist for multi
				user
				TODO : json file for config.audio file

*/
#include <windows.h>
#include <commctrl.h>
#include <wmp.h>
#include <atlbase.h>
#include <atlctl.h>
#include <atlcom.h>
#include <strsafe.h>
#include <iostream>
#include <string>
#include <thread>
#include <filesystem>
#include <conio.h>
#include "../include/opendialogfile.h"
#include "../include/string_utility.h"
#include "../include/console_utility.h"
#include "../include/bool_option.h"
#include "../include/playlist.hpp"

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")



using PtrPlayerType = CComPtr <IWMPPlayer> ;
using PtrControlType = CComPtr<IWMPControls>;
using PtrMediaType = CComPtr<IWMPMedia>;


void get_duration(PtrPlayerType& pPlayer, double& duration) {
	PtrMediaType pMedia;
	duration = 0.0;

	pPlayer->get_currentMedia(&pMedia);
	HRESULT hr;
	if (pMedia) hr = pMedia->get_duration(&duration);

	if (FAILED(hr)) {
		duration = -1.0;
	}
}

void get_currentPosition(PtrControlType& pControls, double& currPos) {
	if (pControls) pControls->get_currentPosition(&currPos);
}

void play_fromPosition(PtrControlType& pControls, double position) {

	if (pControls) {
		pControls->put_currentPosition(position);
		pControls->play();
	}
}

int load_audio(PtrPlayerType& pPlayer,const fs::path& filename, double& duration) {
	BSTR bstr_file_name = SysAllocString(filename.wstring().c_str());

	auto hr = pPlayer->put_URL(bstr_file_name);

	if (FAILED(hr)) {
		std::cerr << "Failed to set URL : 0x" << std::hex << hr << '\n';
		return -1;
	}

	get_duration(pPlayer, duration);
	if (duration == -1.0) duration = 606.0;

	SysFreeString(bstr_file_name);
	return 1;
}

void show_help() {
	std::cout << "\nControls :\n";
	std::cout << "[ S ] Reset\n";
	std::cout << "[ G ] Show\n";
	std::cout << "[ Q ] Exit\n";
	std::cout << "[ N ] Next\n";
	std::cout << "[ P ] Previous\n";
	std::cout << "[ Ctrl+P ] Play\n";
	std::cout << "[ R ] Open Dialog File\n";
	std::cout << "[ A ] Set Auto mode\n";
	std::cout << "[ F ] Set default folder\n";
	std::cout << "[ L ] Load playlist\n";
	std::cout << "[ Ctrl+L ] Load folder\n";
	std::cout << "[ C ] Clear play list\n";
	std::cout << "[ DELETE ] Delete current play\n";
	std::cout << "[ SPACE ] Pause/Play\n";
}


void play_next(PtrPlayerType& pPlayer, PtrControlType& pControls, audio::PlayList& playlist, double& dur) {
	auto audio = playlist.next_item();
	if (load_audio(pPlayer, audio, dur ) == 1) {
		playlist.current_item(audio);
		pControls->stop();
	}
}

void play_previous(PtrPlayerType& pPlayer, PtrControlType& pControls, audio::PlayList& playlist, double& dur) {
	auto audio = playlist.previous_item();
	if (load_audio(pPlayer, audio, dur) == 1) {
		playlist.current_item(audio);
		pControls->stop();
	}
}

bool is_audio_end(PtrPlayerType& pPlayer) {

	if (!pPlayer) return true;

	CComPtr<IWMPPlayer4> pPlayer4;
	auto hr = pPlayer->QueryInterface(&pPlayer4);
	if (FAILED(hr) || !pPlayer4) return true;

	WMPPlayState state;
	hr = pPlayer4->get_playState(&state);
	if (FAILED(hr)) return true;

	switch (state) {
	case wmppsUndefined :
			case wmppsStopped         :
			case wmppsMediaEnded	  :
			case wmppsReady			  :
				return true;

			case wmppsPlaying		  :
			case wmppsPaused		  :
			case wmppsBuffering		  :
			case wmppsTransitioning	  :
			case wmppsReconnecting	  :
			case wmppsScanForward	  :
			case wmppsScanReverse	  :
				return false;
			default:
				return false;
	}
}

int main() {

	std::cout << "----- THIS PROGRAM TEST FOR RUNNING AUDIO USING WIN API ------\n";
	std::cout << "------------------- EDITED BY KADDA AOUES --------------------\n";

	CComPtr<IWMPPlayer>				pPlayer;
	CComPtr<IWMPControls>			pControls;
	audio::PlayList					playlist;
	fs::path						default_folder{};
	fs::path						_CONFIG_FILE{ "config.json" };
	fs::path						_KEY_MAP{ "keymap.json" };

	tool::msOption					bAction;

	struct AUDIO_Key {
		short				key;
		_CONSOLE_KeyEvent_	event{ _CONSOLE_NONE_ };
	};

	std::map<std::string, AUDIO_Key>	mapKey;

	mapKey = {
		{"previous|play",		{ L'P', _CONSOLE_NONE_}},
		{"loadfile|folder",		{ L'L', _CONSOLE_NONE_}},
		{"reset",				{ L'S', _CONSOLE_NONE_}},
		{"show",				{ L'G', _CONSOLE_NONE_}},
		{"quit",				{ L'Q', _CONSOLE_NONE_}},
		{"next",				{ L'N', _CONSOLE_NONE_}},
		{"opendialogfile",		{ L'R', _CONSOLE_NONE_}},
		{"autoplay",			{ L'A', _CONSOLE_NONE_}},
		{"defaultfolder",		{ L'F', _CONSOLE_NONE_}},
		{"clear",				{ L'C', _CONSOLE_NONE_}},
		{"delete",				{VK_DELETE, _CONSOLE_NONE_}},
		{"enter",				{VK_RETURN, _CONSOLE_NONE_}},
		{"pauseplay",			{VK_SPACE, _CONSOLE_NONE_}},
		{"foreward",			{VK_RIGHT, _CONSOLE_NONE_}},
		{"backward",			{VK_LEFT, _CONSOLE_NONE_}},
		{"next_item",			{VK_UP, _CONSOLE_NONE_}},
		{"back_item",			{VK_DOWN, _CONSOLE_NONE_}},
	};

#define _MAP_KEY_EVENT(name_event) mapKey[#name_event].event = console::PressKey(mapKey[#name_event].key, __IsFocus)

	double							duration{};
	double							currPos{};
	double							timeSteps{ 30.0 };
	int								iPlayPause{ 1 };
	int								iListControl{};
	int								count{};
	int								BAR_PROGRESS_LENGTH{ 80 };
	size_t							TIME_LOOP{ 100 };
	size_t							barProgress{20};
	std::string						event_str;
	std::wstring					_APP_TITLE{ L"Audio Player -- Adem --" };

	size_t						_SIZE_AUDIO_EXT{ 3 };
	opendialog::type::ext_spec	_AUDIO_EXTENTION[]{
		{L"Audio ", L"*.mp3;*.wav"},
		{L"MP3  [*.mp3]", L"*.mp3"},
		{L"WAVE [*.wav]", L"*.wav"}
	};


	//------------------------------------- BOOLEAN --------------------------------//
	bAction["auto"] = { false, "Play Music audomatic" };
	bAction["run"] = { true, "run loop for" };
	bAction["show"] = { true, "Show status" };
	bAction["next"] = { false, "next audio selection" };
	bAction["prenext"] = { false, "pre next selection" };
	bAction["changedNumber"] = { false, "" };
	bAction["load"] = { true, "laod play list in begining" };


	HRESULT	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		std::cout << "COM initialization failed.\n";
		return -1;
	}

	hr = CoCreateInstance(__uuidof(WindowsMediaPlayer), NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pPlayer));
	if (FAILED(hr)) {
		std::cout << "Failed to create WMP instance.\n";
		CoUninitialize();
		return -2;
	}

	pPlayer->get_controls(&pControls);

	show_help();

	COORD pos = console::get_cursor_position();

	// REFERENCE FOR PRINTING START
	int LINE_PRINT = pos.Y + 2;
	int LINE_EVENT = LINE_PRINT - 1;
	int LINE_STATE = LINE_PRINT + 1;
	int LINE_BAR = LINE_PRINT + 2;
	int LINE_CURRENT_AUDIO = LINE_PRINT + 3;
	int LINE_LIST = LINE_PRINT + 4;
	int COLOR_CUR_AUDIO = FOREGROUND_RED | COMMON_LVB_REVERSE_VIDEO;
	// APPLICATION TITLE
	SetConsoleTitle(_APP_TITLE.c_str());

	// HIDE CURSOR
	console::CursorHider cursorhider;

	while (bAction["run"].action) {
		// IS VERIABLE IS INVARIANT IN GET EVENT FOR FOCUS WINDOWS
		bool __IsFocus = console::is_focused();

		// LAODING FILE / FOLDER BEGIN
		switch (mapKey["loadfile|folder"].event) {
		case _CONSOLE_KEY_:
			playlist.load_playlist();
			default_folder = playlist.music_folder();
			break;

		case _CONSOLE_CTRL_KEY_:
			auto folder = opendialog::OpenFolder(L"Get Audio Files From Folder", default_folder);
			playlist.load_list_from_folder(folder);
			break;

		}

		mapKey["loadfile|folder"].event = _CONSOLE_NONE_;
		_MAP_KEY_EVENT(loadfile|folder);

		if (bAction["load"].action) {
			mapKey["loadfile|folder"].event = _CONSOLE_KEY_;
			bAction["load"].action = false;
		}
		// LAODING FILE / FOLDER END


		if (playlist.empty()) {
			console::gotoxy(0, LINE_STATE);
			console::print_color("play list is empty", FOREGROUND_RED | FOREGROUND_INTENSITY);
			goto _End_Loop;
		}

		get_currentPosition(pControls, currPos);
		// set progress bars at pos.Y - 1
		barProgress = size_t(BAR_PROGRESS_LENGTH * currPos / duration);
		if (barProgress > BAR_PROGRESS_LENGTH) barProgress = BAR_PROGRESS_LENGTH;
		console::clear_line(LINE_BAR);
		console::print_color(std::string(barProgress, ' '), BACKGROUND_BLUE);

		if (FocusKeyReleased(mapKey["pauseplay"].key)) {
		   iPlayPause = (iPlayPause == 1) ? 2 : 1;
		}
		
		if (FocusKeyPressed(mapKey["foreward"].key)) {
			currPos += timeSteps;
			play_fromPosition(pControls, currPos);
		}

		if (FocusKeyPressed(mapKey["backward"].key)) {
			currPos -= timeSteps;
			play_fromPosition(pControls, currPos);
		}

		if (iPlayPause == 1) {
			event_str = "Play";
			if (pControls) pControls->play();
		}
		if (iPlayPause == 2) {
			event_str = "Pause";
			if (pControls) pControls->pause();
		}

		if (FocusKeyPressed(mapKey["reset"].key)) {
			event_str = "Replay";
			if (pControls) pControls->stop();
			bAction["auto"].action = false;
		}

		// ITEM BROWSING SELECTION DELETE BEGIN
		if (FocusKeyReleased(mapKey["next_item"].key)) {
			++iListControl;
			if (iListControl > (int)playlist.size() - 1) iListControl = playlist.size() - 1;
			bAction["changedNumber"].action = true;
		}

		if (FocusKeyReleased(mapKey["back_item"].key)) {
			--iListControl;
			if (iListControl < 0) iListControl = 0;
			bAction["changedNumber"].action = true;
		}

		if (bAction["changedNumber"].action) {
			// Play audio at 
			if (FocusKeyReleased(mapKey["enter"].key)) {
				load_audio(pPlayer, playlist.at(iListControl), duration);
				pControls->stop();
			}
			// Remove from audio from list at
			if (FocusKeyPressed(mapKey["delete"].key)) {
				playlist.remove_item(iListControl);
				if (!playlist.empty())
					load_audio(pPlayer, playlist.current_item(), duration);
				else goto _End_Loop;
			}
			bAction["changeNumber"].action = false;
		}
		// ITEM BROWSING SELECTION DELETE END

		if (FocusKeyReleased(mapKey["next"].key)) {
			event_str = "Next";
			play_next(pPlayer,pControls, playlist, duration);
			bAction["auto"].action = false;
			console::gotoxy(0, LINE_STATE);
			console::print_color("Auto mode desactivated", FOREGROUND_RED | FOREGROUND_INTENSITY);
		}

		// PREVIOUS / PLAY ACTION BEGIN
		_MAP_KEY_EVENT(previous|play);
		switch (mapKey["previous|play"].event) {
		case _CONSOLE_KEY_ :
			event_str = "Previous";
			play_previous(pPlayer, pControls, playlist, duration);
			bAction["auto"].action = false;
			console::gotoxy(0, LINE_STATE);
			console::print_color("Auto mode desactivated", COLOR_CUR_AUDIO);
			break;

		case _CONSOLE_CTRL_KEY_:
			if (load_audio(pPlayer, playlist.current_item(), duration) == 1) {
				pControls->stop();
			};
			break;
		}
		// PREVIOUS / PLAY ACTION END

		if (FocusKeyReleased(mapKey["defaultfolder"].key)) {
			playlist.set_music_folder();
			default_folder = playlist.music_folder();

			console::gotoxy(0, LINE_STATE);
			console::print_color("set default folder [" + default_folder.string() + "]", COLOR_CUR_AUDIO);
		}

#define MILLIS(Double)    (size_t(Double * 1000))
		// AUTO PLAY
		if (bAction["auto"].action) {
			if (bAction["prenext"].action) {
				if (MILLIS(currPos) == 0) {
					bAction["next"].action = true;
					bAction["prenext"].action = false;
				}
			}

			if (currPos > 1.5) {
				if (!bAction["prenext"].action) {
					if (MILLIS(currPos) != 0) bAction["prenext"].action = true;
				}
			}

			if (bAction["next"].action) {
				play_next(pPlayer, pControls, playlist, duration);
				++count; 
				bAction["next"].action = false;
			}
		}
		// AUTO PLAY END

		if (FocusKeyReleased(mapKey["clear"].key)) {
			pControls->pause();
			pControls->stop();
			pPlayer->close();
			playlist.clear();
		}

	_End_Loop:
		// OPEN DIALOG FOR ADD AUDIO
		if (FocusKeyPressed(mapKey["opendialogfile"].key)) {
			auto file = opendialog::open_file(L"Load New audio file", 
				_AUDIO_EXTENTION, _SIZE_AUDIO_EXT,	default_folder.wstring().c_str());

			if (!file.empty())
				playlist.add_item(file);
			//load_audio(pPlayer, playlist.current_item(), duration);
		}

		// AUTO PLAY ACTIVATION
		if (FocusKeyReleased(mapKey["autoplay"].key)) {
			event_str = "active autoplay";
			bAction["auto"].not_action();

			console::gotoxy(0, LINE_STATE);
			if (bAction["auto"].action) {
				console::print_color("Auto mode activated   ", COLOR_CUR_AUDIO);
			}
			else
				console::print_color("Auto mode desactivated", COLOR_CUR_AUDIO);
		}


		if (bAction["show"].action) {
			console::clear_line(LINE_EVENT);
			console::gotoxy(0, LINE_EVENT);
			std::cout << "Event [" << event_str << "] file N[" << iListControl
				<< "]  Audio Files[" << playlist.size() << "]" << count;

			console::clear_line(LINE_PRINT);
			console::gotoxy(0, LINE_PRINT);
			std::cout << "Current Position : " << sfl::seconds_to_string(currPos)
				<< " / " << sfl::seconds_to_string(duration);

			std::string Current_Audio_title = fs::path(playlist.current_item()).filename().string();
			console::clear_line(LINE_CURRENT_AUDIO);
			console::gotoxy(0, LINE_CURRENT_AUDIO);
			console::print_color("Current[ " + Current_Audio_title + " ]", COLOR_CUR_AUDIO);

			console::clear_line(LINE_LIST, LINE_LIST + 5);
			console::gotoxy(0, LINE_LIST);
			playlist.print_list(iListControl);
		}

		if (FocusKeyPressed(mapKey["quit"].key)) {
			event_str = "Quit";
			playlist.save_playlist();
			pControls.Release();
			bAction["run"].action = false;
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_LOOP));
	}

	std::cout << "------------------- EDITED BY KADDA AOUES --------------------\n\n";
	std::cout << "---------------------------- E N D ---------------------------\n\n";

	pPlayer->close();
	pPlayer.Release();
	pPlayer = nullptr;


	CoUninitialize();

	return 0;
}