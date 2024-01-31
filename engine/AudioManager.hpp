#pragma once
#include <SDL.h>

namespace engine {

	class AudioManager {
	public:
		// Non cloneable
		AudioManager(const AudioManager& other) = delete;

		// Non copyable
		void operator=(const AudioManager& other) = delete;

		static AudioManager& getInstance();

		void initialize();

		void update();

		void playSound(const char* fname);

		void stopSound();

	private:
			AudioManager();
			
			SDL_AudioSpec desiredSpec, obtainedSpec;

			SDL_AudioStream* stream;

			SDL_AudioDeviceID audio_device;

			Uint8* wavBuffer = 0;

			Uint32 wavLength = 0;
	};
}