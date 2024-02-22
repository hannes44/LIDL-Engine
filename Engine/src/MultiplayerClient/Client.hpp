#include <winsock2.h>
#include <string>
#include <functional>

namespace engine {
	enum ClientMessageType {
		StateUpdate,
		CustomMessage
	};

	class Client {
	public:
		static int run(std::function<void(std::string)> onStateUpdate);

	private:
		static std::string GetHeader(ClientMessageType type);
		static bool SocketSend(SOCKET socket, std::string message);
		static bool SendMsg(SOCKET socket, std::string message, ClientMessageType type);
		static std::string ReceiveMsg(SOCKET socket);
		static std::string SendAndReceiveMsg(SOCKET socket, std::string message, ClientMessageType type);
		static std::string CleanChunk(std::string chunk);
		static std::string CleanMsg(std::string message);
		static std::string GetGameState(const std::string& gameName);
	};
}