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
	SOCKET clientSocket = engine::Client::OpenSocket();
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Got invalid socket" << std::endl;
		return;
	}
	std::thread receiver(engine::Client::RunReceiver, clientSocket, onMessage);
	std::thread transmitter(engine::Client::RunTransmitter, clientSocket);
	engine::Client::QueueMessage({ engine::ClientMessageType::CustomMessage, "hello" });
	engine::Client::QueueMessage({ engine::ClientMessageType::CustomMessage, "ping" });

	transmitter.join();
	receiver.join();
}

int main(int argc, char* argv[]) {
	engine::Logger::init();

	//runMultiplayer();
	runEditor();

	return 0;
}