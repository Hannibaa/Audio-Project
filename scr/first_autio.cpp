#include <Windows.h>
#include <iostream>
#include <thread>
#include <fmod.hpp>


#define KeyPressed(Key)            (GetAsyncKeyState(Key) & 0x8000)
#define KeyReleased(Key)           (GetAsyncKeyState(Key) & 0x0001)
#define KeyOn(Key)                 (GetAsyncKeyState(Key) & 0x8001)

int main() {
	std::cout << "Audio Program Start Now\n";

	std::string		audio_filename = R"(C:\Users\Acer\repository\Audio Project\resource\Tetris.mp3)";

	FMOD_RESULT result;

	FMOD::System* pSystem{ nullptr };

	result = FMOD::System_Create(&pSystem);

	pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	FMOD::Sound* pSound = nullptr;

	pSystem->createSound(audio_filename.c_str(), FMOD_DEFAULT, 0, &pSound);

	FMOD::Channel* pChannel = nullptr;

	result = pSystem->playSound(pSound, nullptr, false, &pChannel);

	if (result == FMOD_OK) {
		std::cout << "sound is playing\n";
	}

	bool bIsPlaying = true;
	int i{};

	float frec{}, volume{};
	unsigned int pos{};

	pChannel->getFrequency(&frec);
	pChannel->getVolume(&volume);
	pChannel->getPosition(&pos, FMOD_TIMEUNIT{});


	while (bIsPlaying) {
		pChannel->isPlaying(&bIsPlaying);
		
		pSystem->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));



		if (KeyPressed(L'Q') || KeyPressed(VK_ESCAPE)) break;

		if (KeyPressed(L'V')) {
			volume += 0.1f;
			pChannel->setVolume(volume);
		}
		
		if (KeyPressed(L'C')) {
			volume -= 0.1f;
			pChannel->setVolume(volume);
		}

		if (KeyPressed(L'F')) {
			frec -= 100;
			pChannel->setFrequency(frec);
		}

		if (KeyPressed(L'D')) {
			frec += 100;
			pChannel->setFrequency(frec);
		}

		std::cout << "time " << ++i << "Frequency[" << frec <<"] volume["
			<< volume << "] position[" << pos << "]" << "\r";
	}

	pSound->release();
	pSystem->close();
	pSystem->release();

	std::cout << "\n";
	std::cout << "total iteration : " << i << '\n';
	std::cout << "result ; " << result << "|\n";

	std::cout << "End program...\n";
	return 0;
}

// 1; 358524