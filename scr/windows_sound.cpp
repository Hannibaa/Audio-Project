
#include "../../lib_ka/include/files/opendialogfile.h"
#include "../include/audio_win.hpp"




int main() {
	bool isPlaying{ true };

	std::cout << "----- THIS PROGRAM TEST FOR RUNNING AUDIO USING WIN API ------\n";
	std::cout << "------------------- EDITED BY KADDA AOUES --------------------\n\n";
_start:
	std::cout << "1. Enter file : \n";

	auto default_extension = opendialog::type::default_ext;
	std::wstring music_folders = LR"(C:\Users\Acer\Music)";

	if (!fs::exists(music_folders)) music_folders = L"";

	auto file_name = opendialog::open_file<0>(L"Audio file .mp3 .wav", nullptr, music_folders.c_str());

	if (!file_name.empty()) {
		std::wcout << "Audio : " << file_name << "\n";
		audio::play_audio(file_name, L'Q');
	}

	if (MessageBoxW(NULL,L"Do you like chose another file?", L"Yes/No", MB_YESNO) == IDYES) goto _start;

	std::cout << "------------------- EDITED BY KADDA AOUES --------------------\n\n";
	std::cout << "---------------------------- E N D ---------------------------\n\n";
	
	return 0;
}