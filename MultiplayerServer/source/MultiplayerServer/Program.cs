using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using NetCoreServer;


namespace Server
{
    enum ClientMessageType
    {
        Undefined,
        StateUpdate,
        CustomMessage
    }

    class ChatSession : TcpSession
    {
        public ChatSession(TcpServer server) : base(server) { }

        public static string END_MSG_FLAG = "<%>EOM<%>";
        public static string END_CHUNK_FLAG = "<%>EOC<%>";
        public static string HEADER_MSG_FLAG = "<!>";
        public static int BUF_SIZE = 1000;
        public static int PORT = 11111;

        public static bool DEBUG = false;

        string messageBuffer = "";

        static protected (ClientMessageType, string) SplitHeader(string message)
        {
            if (!message.Contains(HEADER_MSG_FLAG))
            {
                Console.WriteLine($"Message missing header: {message}");
                return (ClientMessageType.Undefined, "");
            }

            string body = message.Substring(message.LastIndexOf(HEADER_MSG_FLAG) + HEADER_MSG_FLAG.Length);

            string header = message.Substring(message.IndexOf(HEADER_MSG_FLAG), message.LastIndexOf(HEADER_MSG_FLAG)).Trim().Replace(HEADER_MSG_FLAG, "");
            switch (header)
            {
                case "StateUpdate":
                    return (ClientMessageType.StateUpdate, body);
                case "CustomMessage":
                    return (ClientMessageType.CustomMessage, body);
                default:
                    Console.WriteLine($"Error: Unhandeled ClientMessageType: {header}");
                    return (ClientMessageType.Undefined, body);
            }
        }

        static protected string CleanChunk(string chunk)
        {
            return Regex.Replace(chunk, @$"{END_CHUNK_FLAG}\s*", "");
        }

        static protected string CleanMsg(string message)
        {
            return Regex.Replace(message, @$"{END_MSG_FLAG}\s*", "");
        }

        protected override void OnConnected()
        {
            Console.WriteLine($"Client with ID {Id} connected!");
        }

        protected override void OnDisconnected()
        {
            Console.WriteLine($"Client with ID {Id} disconnected!");
        }

        protected override void OnReceived(byte[] buffer, long offset, long size)
        {
            string message = CleanChunk(Encoding.ASCII.GetString(buffer, (int)offset, (int)size));

            messageBuffer += message;
            if (!messageBuffer.Contains(END_MSG_FLAG))
                return;

            (ClientMessageType messageType, string body) = SplitHeader(CleanMsg(messageBuffer));
            if (DEBUG)
                Console.WriteLine($"<- MessageType: {messageType}, body: {body}");
            
            messageBuffer = "";

            string response = null;

            switch (messageType)
            {
                case ClientMessageType.Undefined:
                    Console.WriteLine("Undefined message type, ignoring...");
                    return;
                case ClientMessageType.StateUpdate:
                    response = body;
                    break;
                case ClientMessageType.CustomMessage:
                    response = body switch
                    {
                        "ping" => "pong",
                        "hello" => "world",
                        _ => "Unknown command",
                    };
                    break;
                default:
                    return;
            }

            // Multicast message to all connected sessions except the sender
            Server.Multicast(response, Id);
            //Server.Multicast(response);

            // If the buffer starts with '!' the disconnect the current session
            if (message == "!")
                Disconnect();
        }

        protected override void OnError(SocketError error)
        {
            Console.WriteLine($"Client with ID {Id} caught an error with code {error}");
        }
    }

    class ChatServer : TcpServer
    {
        public ChatServer(IPAddress address, int port) : base(address, port) { }

        protected override TcpSession CreateSession() { return new ChatSession(this); }

        // Splits a string into chunks up to a given max size
        private static IEnumerable<string> Chunk(string str, int maxChunkSize)
        {
            for (int i = 0; i < str.Length; i += maxChunkSize)
                yield return str.Substring(i, Math.Min(maxChunkSize, str.Length - i));
        }

        // Override the multicast to chunk the message and always end it with the flag
        public override bool Multicast(string message, Guid? excludeSession = null)
        {
            if (message.Contains(ChatSession.END_MSG_FLAG))
            {
                Console.WriteLine($"Cannot send message. Message contains the EOM flag: {ChatSession.END_MSG_FLAG}");
                return false;
            }

            message += ChatSession.END_MSG_FLAG;

            int chunkSize = ChatSession.BUF_SIZE - ChatSession.END_CHUNK_FLAG.Length - 1;

            List<string> chunks = Chunk(message, chunkSize).ToList();

            for (int i = 0; i < chunks.Count; i++)
            {
                string msg = chunks[i];

                byte[] chunk = Encoding.ASCII.GetBytes(msg + ChatSession.END_CHUNK_FLAG + '\0');
                base.Multicast(chunk, excludeSession);
            }

            return true;
        }

        protected override void OnError(SocketError error)
        {
            Console.WriteLine($"Server caught an error with code {error}");
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            IPAddress ip = IPAddress.Any;
            int port = ChatSession.PORT;

            // Overwrite port if passed as an argument
            if (args.Length > 0)
                port = int.Parse(args[0]);


            // Create a new TCP chat server
            var server = new ChatServer(ip, port);

            // Start the server
            Console.WriteLine("Starting server...");
            server.Start();
            Console.WriteLine($"Server started on {ip}:{port}");

            Console.WriteLine("Press Enter to stop the server or '!' to restart the server...");

            // Perform text input
            for (; ; )
            {
                string line = Console.ReadLine();
                if (string.IsNullOrEmpty(line))
                    break;

                // Restart the server
                if (line == "!")
                {
                    Console.Write("Server restarting...");
                    server.Restart();
                    Console.WriteLine("Done!");
                    continue;
                }
            }

            // Stop the server
            Console.Write("Server stopping...");
            server.Stop();
            Console.WriteLine("Done!");
        }
    }
}
