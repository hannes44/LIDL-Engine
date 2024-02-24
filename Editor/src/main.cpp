#include "Editor.hpp"
#include "MultiplayerClient/Client.hpp"
#include <stdio.h>
#include <string>
#include <thread>

void runEditor() {
	engine::Editor editor{};
	editor.start();
}

void onMessage(std::string msg) {
	std::cout << "New Message:" << std::endl;
	std::cout << msg << std::endl;
}

void runMultiplayer() {
	std::thread client(engine::Client::Run, onMessage);
	engine::Client::QueueMessage({ engine::ClientMessageType::CustomMessage, "hello" });
	engine::Client::QueueMessage({ engine::ClientMessageType::CustomMessage, "ping" });

	client.join();
}

int main(int argc, char* argv[]) {
	engine::Logger::init();

	//runMultiplayer();
	runEditor();

	return 0;
}