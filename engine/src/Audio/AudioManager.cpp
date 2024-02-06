#include "AudioManager.hpp"
#include "Core/Logger.hpp"
#include "Core/Window.hpp"
#include <Windows.h>
#include <MMSystem.h>

namespace engine {

    AudioManager::AudioManager() {
	}

    void AudioManager::initialize() {
	}

    AudioManager& AudioManager::getInstance() {
        static AudioManager instance;
        return instance;
    }

    void AudioManager::update() {
        
	}
    
    void AudioManager::playSound(const char* fname) {
        const char file_location[100] = "../../assets/sounds/";
        strcat((char*)file_location, fname);
        PlaySoundA((LPCSTR)(file_location), NULL, SND_ASYNC);
    }

    void AudioManager::stopSound() {
        PlaySoundA(NULL, 0, 0);
	}

}