#include <winsock2.h>
#include <string>

namespace engine {
	class Client {
	public:
		static int run();

	private:
		static bool SocketSend(SOCKET socket, std::string message);
		static bool SendMsg(SOCKET socket, std::string message);
		static std::string ReceiveMsg(SOCKET socket);
		static std::string SendAndReceiveMsg(SOCKET socket, std::string message);
		static std::string CleanMsg(std::string message);
		static std::string GetGameState(const std::string& gameName);
	};
}