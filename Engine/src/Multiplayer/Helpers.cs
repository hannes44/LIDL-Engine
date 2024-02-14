using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Linq;
// Needed for Console
using System;

public static class Helpers {

    const int BUF_SIZE = 1024;
    const string END_MSG_FLAG = "<EOF>";
    const string IP_ADDRESS = "127.0.0.1";
    public enum FlagMessage {
        ACK = 'A',
        END = 'E'
    }

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

        try {
            // Using the `Bind()` method associates a network address with the Server socket
            socket.Bind(localEndPoint);

            // Using the `Listen()` method creates a Client list that can connect to the Server
            socket.Listen(10);

            return socket.Accept();
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

    public static void SendMessage(Socket socket, string message) {
        byte[] messageSent = Encoding.ASCII.GetBytes(message + END_MSG_FLAG);
        // TODO: split msg in max BUF_SIZE payloads
        socket.Send(messageSent);
        Console.WriteLine("-> {0}", message);
    }

    public static void SendMessage(Socket socket, FlagMessage flagMessage) {
        SendMessage(socket, flagMessage.ToString());
    }

    public static string ReceiveMessage(Socket socket) {
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

    public static string SendAndReceive(Socket socket, string message) {
        SendMessage(socket, message);
        string response = ReceiveMessage(socket);
        Console.WriteLine("{0} -> {1}", message, response);
        
        return response;
    }
}