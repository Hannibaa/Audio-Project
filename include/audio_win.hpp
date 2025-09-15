/*
				Title		:	Audio Header file using Window Media Player, and Windows Api

				Name		:
				Author		:	Kadda Aoues
				Date		:
				Statu		:	On Development
				Version		:
				Copyright	:

				Discription :
				

				TODO :

*/
#include <Windows.h>

#include <wmp.h>
#include <string>
#include <thread>
#include <iostream>
#include <filesystem>
#include "../../lib_ka/include/string/string_utility.h"
#include "../../lib_ka/include/system/console_utility.h"


#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "winmm.lib")



namespace fs = std::filesystem;


namespace audio {
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Helper function for windows titles associated with special function
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void setTitle(wchar_t* buffer, size_t size, std::chrono::milliseconds* pms, short VKey) {
		while (true) {
			auto wstr1 = sfl::conv::str_to_wstr(sfl::to_string(*pms, sfl::STR_MIN_SEC_MS));
			auto wstr2 = sfl::conv::str_to_wstr(sfl::to_string(*(pms + 1), sfl::STR_MIN_SEC_MS));
			swprintf_s(buffer, size, L"Progress [%s] Total [%s]", wstr1.c_str(), wstr2.c_str());
			SetConsoleTitleW(buffer);
			if (KeyPressed(VKey)) break;
		}

	}


	// this first call for functionality with window media player WMP
	int play_audio(const std::wstring& filename, short Key = 0) {
		CoInitialize(NULL);

		//std::wstring	file_name = LR"(C:\Users\Acer\repository\Audio Project\resource\Tetris.mp3)";

		IWMPPlayer* pPlayer = NULL;

		CoCreateInstance(__uuidof(WindowsMediaPlayer), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pPlayer));

		BSTR bstrFile = SysAllocString(filename.c_str());

		pPlayer->put_URL(bstrFile);

		MessageBoxW(NULL, L"Click Ok to stop", L"Stop", MB_OK);

		pPlayer->close();
		pPlayer->Release();
		SysFreeString(bstrFile);

		CoUninitialize();
		return 0;
	}


	// audio using msi 
	int play_audioMsi(const fs::path& file_name,
		std::chrono::milliseconds& duration,
		std::chrono::milliseconds& progress, 
		short Key = VK_ESCAPE)
	{
		std::wstring	command;
		std::wstring	alias = L"myAudio";

		// Open audio file
		command = L"open \"" + file_name.wstring() + L"\" alias " + alias;
		if (mciSendStringW(command.c_str(), NULL, 0, NULL) != 0) {
			std::wcerr << L"Failed to open audio file.\n";
			return -1;
		}

		// Get total length in milliseconds
		wchar_t lengthBuffer[128] = { 0 };
		mciSendStringW(L"set myAudio time format milliseconds", NULL, 0, NULL);
		if (mciSendStringW(L"status myAudio length", lengthBuffer, sizeof(lengthBuffer), NULL) != 0) {
			std::wcerr << L"Failed to get audio length.\n";
			return -2;
		}

		int totalLengthms = _wtoi(lengthBuffer);
		duration = std::chrono::milliseconds(totalLengthms);

		if (totalLengthms == 0) {
			std::wcerr << L"Invalid length.\n";
			mciSendStringW(L"close myAudio", NULL, 0, NULL);
			return -3;
		}

		// Start palyback
		mciSendStringW(L"play myAudio", NULL, 0, NULL);
		std::cout << "Audio Playing : " << file_name << '\n';

		int lastPercent = -1;

		// Loop until playback finishes
		while (true) {
			wchar_t posBuffer[128] = { 0 };
			mciSendStringW(L"status myAudio position", posBuffer, sizeof(posBuffer), NULL);
			int currentPosms = _wtoi(posBuffer);
			float fprogress = float(currentPosms) / float(totalLengthms);
			progress = std::chrono::milliseconds(currentPosms);

			int percent = static_cast<int>(fprogress * 100.f);
			if (percent != lastPercent) {
				std::cout << "\rProgress: " << percent << " %\r" << std::flush;
				lastPercent = percent;
			}

			// check if finish
			if (percent > 99) {
				break;
			}

			// to stop
			if (KeyPressed(Key)) break;

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		std::cout << "\nPlayback finish.\n";

		// Close the file
		mciSendStringW(L"close myAudio", NULL, 0, NULL);

		return 0;
	}

}
