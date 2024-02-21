using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Linq;
using System.Collections.Generic;
// Needed for Console
using System;

public static class Helpers {

    const int BUF_SIZE = 1024;
    const string END_MSG_FLAG = "<%>EOM<%>";
    const string IP_ADDRESS = "127.0.0.1";

    public static bool CloseSocket(Socket socket) {
        try {
            socket.Shutdown(SocketShutdown.Both);
            socket.Close();
            return true;
        }
        catch (SocketException se) {
            Console.WriteLine("SocketException : {0}", se.ToString());
            return false;
        }

        catch (Exception e) {
            Console.WriteLine("Unexpected exception : {0}", e.ToString());
            return false;
        }
    }

    public static Socket OpenServerSocket(int port) {
        (IPEndPoint localEndPoint, Socket socket) = CreateSocket(port);
        
        if (socket == null) {
            Console.WriteLine("Socket is null");
            return null;
        }

        try {
            // Using the `Bind()` method associates a network address with the Server socket
            socket.Bind(localEndPoint);

            // Using the `Listen()` method creates a Client list that can connect to the Server
            socket.Listen(10);

            return socket;
        }
        catch (Exception e) {
            Console.WriteLine(e.ToString());
        }

        return null;
    }

    public static Socket OpenClientSocket(int port) {
        (IPEndPoint localEndPoint, Socket socket) = CreateSocket(port);

        try {
            // Connect the socket to the remote endpoint
            socket.Connect(localEndPoint);
            Console.WriteLine("Socket connected to -> {0} ", socket?.RemoteEndPoint?.ToString());
            
            return socket;
        }
        catch (ArgumentNullException ane) {
            Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
        }
        catch (SocketException se) {
            Console.WriteLine("SocketException : {0}", se.ToString());
        }
        catch (Exception e) {
            Console.WriteLine("Unexpected exception : {0}", e.ToString());
        }

        return null;
    }
    
    private static (IPEndPoint, Socket) CreateSocket(int port) {
        try {
            // Establish the remote endpoint for the socket
            IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
            
            // Unused for now - gets one of the available IPv4 addresses. Currently uses the hardcoded IP_ADDRESS instead
            //IPAddress ipAddr = ipHost.AddressList.Where(addr => addr.AddressFamily == AddressFamily.InterNetwork).First();
            IPAddress ipAddr = IPAddress.Parse(IP_ADDRESS);
            
            IPEndPoint localEndPoint = new IPEndPoint(ipAddr, port);

            Console.WriteLine("Creating IPv4 socket on {0}:{1}", ipAddr.ToString(), port);

            Socket socket = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);

            return (localEndPoint, socket);
        }
        catch (Exception e) {
            Console.WriteLine(e.ToString());
        }

        return (null, null);
    }

    public static string CleanMsg(string message) {
        return message.Replace(END_MSG_FLAG, "");
    }

    // Splits a string into chunks up to a given max size
    private static IEnumerable<string> Chunk(string str, int maxChunkSize) {
        for (int i = 0; i < str.Length; i += maxChunkSize)
            yield return str.Substring(i, Math.Min(maxChunkSize, str.Length-i));
    }

    public static void SendMessage(Socket socket, string message) {
        if (message.Contains(END_MSG_FLAG)) {
            Console.WriteLine("Cannot send message. Message contains the EOM flag: {0}", END_MSG_FLAG);
            return;
        }
        
        const int chunkSize = BUF_SIZE;
        
        List<string> chunks = Chunk(message, chunkSize).ToList();
        
        for (int i = 0; i < chunks.Count(); i++) {
            string msg = chunks[i];
            
            byte[] messageSent = Encoding.ASCII.GetBytes(msg);
            socket.Send(messageSent);
            Console.WriteLine("[{0}] -> {1}", i, msg);
        }

        socket.Send(Encoding.ASCII.GetBytes(END_MSG_FLAG));
    }

    public static string ReceiveMessage(Socket socket) {
        try {
            // Data buffer
            byte[] bytes = new Byte[BUF_SIZE];
            string data = null;
            
            while (true) {
                int numByte = socket.Receive(bytes);

                data += Encoding.ASCII.GetString(bytes, 0, numByte);

                if (data.IndexOf(END_MSG_FLAG) > -1)
                    break;
            }

            data = CleanMsg(data);
            Console.WriteLine("<- {0}", data);

            return data;
        }
        catch (SocketException se) {
            Console.WriteLine("SocketException : {0}", se.ToString());
            return null;
        }
        catch (Exception e) {
            throw;
        }
        
        return null;
    }

    public static string SendAndReceive(Socket socket, string message) {
        SendMessage(socket, message);
        string response = ReceiveMessage(socket);
        Console.WriteLine("{0} -> {1}", message, response);
        
        return response;
    }
}