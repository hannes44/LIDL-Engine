#include "AudioManager.hpp"
#include "Logger.hpp"

namespace engine {

    AudioManager::AudioManager() {
	}

    void AudioManager::initialize() {
        desiredSpec.freq = 48000;
        desiredSpec.format = SDL_AUDIO_S16;
        desiredSpec.channels = 2;
        stream = SDL_CreateAudioStream(&desiredSpec, &obtainedSpec);
        if (!stream) {
			LOG_INFO("COULD NOT CREATE AUDIO STREAM");
		}
        audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, NULL);
        SDL_BindAudioStream(audio_device, stream);
	}

    AudioManager& AudioManager::getInstance() {
        static AudioManager instance;
        return instance;
    }

    void AudioManager::update() {
        
	}

    void AudioManager::playSound(const char* fname) {
        if (SDL_LoadWAV(fname, &desiredSpec, &wavBuffer, &wavLength)) {
            SDL_PutAudioStreamData(stream, wavBuffer, wavLength);
			SDL_free(wavBuffer);
        }
        else {
			LOG_INFO("COULD NOT LOAD WAW FILE : {}", fname);
		}
    }

    void AudioManager::stopSound() {

	}

}