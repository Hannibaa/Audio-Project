#include <Windows.h>
#include <iostream>
#include <thread>

#include <filesystem>
#include <fmod.hpp>


#define KeyPressed(Key)            (GetAsyncKeyState(Key) & 0x8000)
#define KeyReleased(Key)           (GetAsyncKeyState(Key) & 0x0001)
#define KeyOn(Key)                 (GetAsyncKeyState(Key) & 0x8001)

#define	SPECTRUM_SIZE				64
#define BAR_LENGTH					80

namespace fm = FMOD;
namespace fs = std::filesystem;

void ERRCHECK(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		std::cerr << "FMOD error! (" << result << ") \n";
		exit(-1);
	}
}

int play_spectrum(const std::string& audio_file_name = "") {

	fs::path		audio_filename = R"(C:\Users\Acer\repository\Audio Project\resource\Tetris.mp3)";

	if (!audio_file_name.empty()) {
		audio_filename.replace_filename(audio_file_name);
	}

	

	fm::System* system{ nullptr };
	fm::Sound* sound{ nullptr };
	fm::Channel* channel{ nullptr };
	fm::DSP* fftDSP{ nullptr };

	// 1. Create system and initializing
	auto result = fm::System_Create(&system);
	ERRCHECK(result);

	result = system->init(512, FMOD_INIT_NORMAL, nullptr);
	ERRCHECK(result);

	// 2. Load audio file from disk
	result = system->createSound(audio_filename.string().c_str(), FMOD_2D, nullptr, &sound);
	ERRCHECK(result);

	// 3. Play sound
	result = system->playSound(sound, nullptr, false, &channel);

	// 3.1 Create and configure FFT DSP
	system->createDSPByType(FMOD_DSP_TYPE_FFT, &fftDSP);
	fftDSP->setParameterInt(FMOD_DSP_FFT_WINDOW, FMOD_DSP_FFT_WINDOW_HAMMING);
	fftDSP->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 1024);

	// 3.2 Attach to master channel group
	fm::ChannelGroup* masterGroup{ nullptr };
	system->getMasterChannelGroup(&masterGroup);
	masterGroup->addDSP(0, fftDSP);


	std::cout << "sound is playing\n";

	// 4. continuously get spectrum data
	float spectrum[SPECTRUM_SIZE];

	while (true) {
		bool isPlaying = false;
		channel->isPlaying(&isPlaying);
		if (!isPlaying) break;

		system->update();

		void* data{ nullptr };
		unsigned int length = 0;

		result = fftDSP->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, &data, &length, nullptr, 0);

		if (result == FMOD_OK && data) {
			auto fft = (FMOD_DSP_PARAMETER_FFT*)data;

			// Wait untill data is ready
			if (fft->length > 0) {
				int numChannels = fft->numchannels;

				for (int i = 0; i < 16; ++i) {
					float value = fft->spectrum[0][i];
					int bars = static_cast<int>(value * BAR_LENGTH);
					//for (int j = 0; j < bars; ++j) std::cout << "|";
					std::cout << "[" << i + 1 << "]" << std::string(bars, '|') << '\n' << '\r';
					std::cout << "[" << i + 1 << "]" << std::string(bars, '|') << '\n' << '\r';
				}
				std::cout.flush().flush();
			}
		}

		if (KeyPressed(VK_ESCAPE)) break;

		Sleep(50);// refresh every 50 ms
	}

	// 5. Clean up 
	ERRCHECK(sound->release());
	fftDSP->release();
	ERRCHECK(system->close());
	ERRCHECK(system->release());

	return 0;
}


int main() {

	std::cout << "Audio Console Spectrum Program\n";
	std::cout << "Manual :\n";
	std::cout << "\tPlace any audio file in directory named resource\n";

_start:
	std::string file_name{}; 
	
	std::cout << "enter file audio to get displayed :";
	
	std::getline(std::cin, file_name);

	play_spectrum(file_name);

	std::cout << "Do you like to play another audio (Y/N)";

	char yn{};
	std::cin >> yn;

	if (yn == 'Y' || yn == 'y') {
		std::cin.get();
		goto _start;
	}

	
	std::cout << "End\n";
	std::cin.get();
	return 0;
}