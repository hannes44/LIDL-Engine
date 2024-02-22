#include "Editor.hpp"
#include "Multiplayer/Client/Client.hpp"
#include <stdio.h>

void runEditor() {
	engine::Editor editor{};
	editor.start();
}

void onMessage(std::string msg) {
	std::cout << "New Message:" << std::endl;
	std::cout << msg << std::endl;
}

void runMultiplayer() {
	engine::Client::run(onMessage);
}

int main(int argc, char* argv[]) {
	engine::Logger::init();

	//runMultiplayer();
	runEditor();

	return 0;
}