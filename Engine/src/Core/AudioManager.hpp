#pragma once

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