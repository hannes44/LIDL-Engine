#include "AudioManager.hpp"
#include "Core/Logger.hpp"
#include "Core/Window.hpp"

#define _WINSOCKAPI_
#define NOMINMAX
#include <Windows.h>

#include <MMSystem.h>


namespace engine {

	AudioManager::AudioManager() {
	}

	AudioManager& AudioManager::getInstance() {
		static AudioManager instance;
		return instance;
	}

	void AudioManager::playSound(const std::string& fileName) {
		std::string soundAssetPath = ResourceManager::getInstance()->getPathToGameResource(fileName);
		PlaySoundA((LPCSTR)(soundAssetPath.c_str()), NULL, SND_ASYNC);
	}

	void AudioManager::stopSound() {
		PlaySoundA(NULL, 0, 0);
	}

}