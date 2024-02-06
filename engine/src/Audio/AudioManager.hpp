#pragma once

/*
	AudioManager class | Manages audio playback
	Use function playSound("filename") to play a sound
	Use function stopSound() to stop any sound playing
	All sounds must be in the assets/sounds folder
*/

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

	};
}