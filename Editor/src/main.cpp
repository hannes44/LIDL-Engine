#include "Editor.hpp"
#include "Multiplayer/Client/Client.hpp"

void runEditor() {
	engine::Editor editor{};
	editor.start();
}

void runMultiplayer() {
	engine::Client::run();
}

int main(int argc, char* argv[]) {
	engine::Logger::init();

	//runMultiplayer();
	runEditor();

	return 0;
}