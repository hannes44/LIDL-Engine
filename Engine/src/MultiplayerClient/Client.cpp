#include "Client.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <queue>

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fstream>

#include "Core/Logger.hpp"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 11111
#define IP_ADDR "127.0.0.1"
#define BUF_SIZE 1000

#define GAME_FOLDER_PATH "../../Games/"
#define GAME_CONFIG_FILE_EXTENSION ".yaml"

namespace engine {
	const std::string END_MSG_FLAG = "<%>EOM<%>";
	const std::string END_CHUNK_FLAG = "<%>EOC<%>";
	const std::string HEADER_MSG_FLAG = "<!>";

	SafeQueue<ClientMessage> Client::messageQueue = SafeQueue<ClientMessage>();

	SOCKET Client::OpenSocket() {
		WSADATA wsaData;
		int wserr;
		WORD wVersionRequested = MAKEWORD(2, 2);
		wserr = WSAStartup(wVersionRequested, &wsaData);

		if (wserr != 0) {
			std::cout << "Error: The winsock dll was not found!" << std::endl;
			return INVALID_SOCKET;
		}

		// Create socket
		SOCKET clientSocket = INVALID_SOCKET;
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET) {
			std::cout << "Error during socket creation: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return INVALID_SOCKET;
		}

		// Connect to server
		SOCKADDR_IN clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_port = htons(PORT);
		clientService.sin_addr.s_addr = inet_addr(IP_ADDR);

		if (connect(clientSocket, PSOCKADDR(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
			std::cout << "Socket error: could not connect to server: " << WSAGetLastError() << std::endl;
			std::cout << "Make sure the server is running on the specified IP and port" << std::endl;
			WSACleanup();
			return INVALID_SOCKET;
		}
		else {
			std::cout << "Socket connected to server!" << std::endl;
		}

		return clientSocket;
	}

	void Client::RunReceiver(SOCKET clientSocket, std::function<void(std::string)> onMessage) {
		while (true) {
			std::cout << "Waiting to receive..." << std::endl;
			std::string response = ReceiveMsg(clientSocket);
			onMessage(response);
		}
	}

	void Client::RunTransmitter(SOCKET clientSocket) {
		while (true) {
			ClientMessage msg;

			std::cout << "Waiting to transmit..." << std::endl;
			msg = messageQueue.dequeue();

			SendMsg(clientSocket, msg.message, msg.type);
		}
	}

	void Client::QueueMessage(ClientMessage message) {
		messageQueue.enqueue(message);
	}

	std::string Client::GetHeader(ClientMessageType type) {
		std::string header = HEADER_MSG_FLAG;
		switch (type) {
		case StateUpdate:
			header += "StateUpdate";
			break;
		case CustomMessage:
			header += "CustomMessage";
			break;
		default:
			LOG_FATAL("Unhandeled ClientMessageType: {}", type);
			return "";
		}

		return header + HEADER_MSG_FLAG;
	}

	bool Client::SocketSend(SOCKET socket, std::string message) {
		int sbyteCount = send(socket, message.c_str(), message.size(), 0);

		if (sbyteCount == SOCKET_ERROR) {
			LOG_FATAL("Server send error: {}", WSAGetLastError());
			return false;
		}

		return true;
	}

	bool Client::SendMsg(SOCKET socket, std::string message, ClientMessageType type) {
		if (message.find(END_MSG_FLAG) != std::string::npos) {
			std::cout << "Cannot send message. Message contains the EOM flag: " << END_MSG_FLAG << std::endl;
			return false;
		}

		const int chunkSize = BUF_SIZE - END_CHUNK_FLAG.length();

		std::cout << "-> " << message << std::endl;

		std::vector<std::string> chunks{};

		for (int i = 0; i < message.length(); i += chunkSize)
			chunks.push_back(message.substr(i, chunkSize));

		SocketSend(socket, GetHeader(type));

		for (int i = 0; i < chunks.size(); i++) {
			std::string chunk = chunks[i] + END_CHUNK_FLAG;
			SocketSend(socket, chunk);
		}

		SocketSend(socket, END_MSG_FLAG);
	}

	std::string Client::ReceiveMsg(SOCKET socket) {
		char buf[BUF_SIZE + 1];
		std::string data = "";

		while (true) {
			std::fill(buf, buf + BUF_SIZE + 1, 0);
			recv(socket, buf, BUF_SIZE + 1, 0);
			std::string received(buf);

			data += CleanChunk(received);

			if (data.find(END_MSG_FLAG) != std::string::npos)
				break;
		}

		data = CleanMsg(data);

		std::cout << "<- " << data << std::endl;

		return data;
	}

	std::string Client::SendAndReceiveMsg(SOCKET socket, std::string message, ClientMessageType type) {
		if (SendMsg(socket, message, type))
			return ReceiveMsg(socket);

		return "";
	}

	std::string Client::CleanMsg(std::string message) {
		return std::regex_replace(message, std::regex(END_MSG_FLAG + "\s*"), "");
	}

	std::string Client::CleanChunk(std::string chunk) {
		return std::regex_replace(chunk, std::regex(END_CHUNK_FLAG + "\s*"), "");
	}

	std::string Client::GetGameState(const std::string& gameName) {
		std::string filePath = GAME_FOLDER_PATH + gameName + "/" + gameName + "State" + GAME_CONFIG_FILE_EXTENSION;

		if (!std::ifstream(filePath).good()) {
			LOG_ERROR("File not found: " + filePath);
			return "";
		}

		std::ostringstream text;
		std::ifstream in_file(filePath);

		text << in_file.rdbuf();
		return text.str();
	}
}
