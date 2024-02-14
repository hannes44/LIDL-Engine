#include "Client.hpp"
#include <string>
#include <iostream>
#include <regex>

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 11111
#define IP_ADDR "127.0.0.1"
#define BUF_SIZE 1024

namespace engine {
	const std::string END_MSG_FLAG = "<EOF>";

	int Client::run() {
		WSADATA wsaData;
		int wserr;
		WORD wVersionRequested = MAKEWORD(2, 2);
		wserr = WSAStartup(wVersionRequested, &wsaData);

		if (wserr != 0) {
			std::cout << "Error: The winsock dll was not found!" << std::endl;
			return 0;
		}

		// Create socket
		SOCKET clientSocket = INVALID_SOCKET;
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET) {
			std::cout << "Error during socket creation: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return 0;
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
			return 0;
		}
		else {
			std::cout << "Socket connected to server!" << std::endl;
		}

		while (true) {
			std::string msg;
			printf("Enter the message to send to the server: ");
			std::getline(std::cin, msg);
			// TODO: split msg in max BUF_SIZE payloads
			SendMsg(clientSocket, msg);
			std::string response = ReceiveMsg(clientSocket);
		}

	}

	bool Client::SendMsg(SOCKET socket, std::string message) {
		std::string payload = message + END_MSG_FLAG;

		std::cout << "-> " << message << std::endl;

		int sbyteCount = send(socket, payload.c_str(), payload.size(), 0);

		if (sbyteCount == SOCKET_ERROR) {
			std::cout << "Server send error: " << WSAGetLastError() << std::endl;
			return false;
		}
		else {
			return true;
		}
	}

	std::string Client::ReceiveMsg(SOCKET socket) {
		char buf[BUF_SIZE];
		std::string data = "";

		while (true) {
			std::fill(buf, buf + BUF_SIZE, 0);
			recv(socket, buf, BUF_SIZE, 0);
			std::string received(buf);

			data += received;

			if (data.find(END_MSG_FLAG) != std::string::npos)
				break;
		}

		data = CleanMsg(data);

		std::cout << "<- " << data << std::endl;

		return data;
	}

	std::string Client::SendAndReceiveMsg(SOCKET socket, std::string message) {
		if (SendMsg(socket, message))
			return ReceiveMsg(socket);

		return NULL;
	}

	std::string Client::CleanMsg(std::string message) {
		return std::regex_replace(message, std::regex(END_MSG_FLAG), "");
	}
}
