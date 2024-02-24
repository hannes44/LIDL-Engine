#pragma once

#include <winsock2.h>
#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace engine {
	template <class T>
	class SafeQueue
	{
	public:
		SafeQueue()
			: q()
			, m()
			, c()
		{}

		~SafeQueue()
		{}

		void clear()
		{
			std::lock_guard<std::mutex> lock(m);
			std::queue<T> empty;
			std::swap(q, empty);
		}

		// Add an element to the queue.
		void enqueue(T t)
		{
			std::lock_guard<std::mutex> lock(m);
			q.push(t);
			c.notify_one();
		}

		// Get the "front"-element.
		// If the queue is empty, wait till a element is avaiable.
		T dequeue(void)
		{
			std::unique_lock<std::mutex> lock(m);
			while (q.empty())
			{
				// release lock as long as the wait and reaquire it afterwards.
				c.wait(lock);
			}
			T val = q.front();
			q.pop();
			return val;
		}

	private:
		std::queue<T> q;
		mutable std::mutex m;
		std::condition_variable c;
	};

	enum ClientMessageType {
		StateUpdate,
		CustomMessage
	};

	struct ClientMessage {
		ClientMessageType type;
		std::string message;
	};

	class Client {
	public:
		static int Run(std::function<void(std::string)> onStateUpdate);
		static void QueueMessage(ClientMessage message);

	private:
		static std::mutex m;
		static SafeQueue<ClientMessage> messageQueue;

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