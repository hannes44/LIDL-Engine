#include "AudioManager.hpp"
#include "Logger.hpp"
#include "Window.hpp"

namespace engine {

    AudioManager::AudioManager() {
	}

    void AudioManager::initialize() {
        desiredSpec.freq = 44100;
        desiredSpec.format = SDL_AUDIO_S16;
        desiredSpec.channels = 2;
        obtainedSpec.freq = 44100;
        obtainedSpec.format = SDL_AUDIO_S16;
        obtainedSpec.channels = 2;
        stream = SDL_CreateAudioStream(&desiredSpec, &obtainedSpec);
        if (!stream) {
			LOG_INFO("Could not create audio stream : {}", SDL_GetError());
		}
        audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, NULL);
        if (!audio_device) {
            LOG_INFO("Could not open audio device : {}", SDL_GetError());
        }
        if (!SDL_BindAudioStream(audio_device, stream)) {
            LOG_INFO("Could not bind audio stream : {}", SDL_GetError());
        }
	}

    AudioManager& AudioManager::getInstance() {
        static AudioManager instance;
        return instance;
    }

    void AudioManager::update() {
        
	}

    void AudioManager::playSound(const char* fname) {
        if (SDL_LoadWAV(fname, &desiredSpec, &wavBuffer, &wavLength) == 0) {
            SDL_PutAudioStreamData(stream, wavBuffer, wavLength);
			SDL_free(wavBuffer);
        }
        else {
			LOG_INFO("COULD NOT LOAD WAW FILE : {} | Error : {} ", fname , SDL_GetError());
		}
    }

    void AudioManager::stopSound() {

	}

}