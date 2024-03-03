#include "Editor.hpp"
#include "MultiplayerClient/Client.hpp"
#include <stdio.h>
#include <string>
#include <thread>

void runEditor() {
	engine::Editor editor{};
	editor.start();
}

int main(int argc, char* argv[]) {
	engine::Logger::init();

	runEditor();

	return 0;
}