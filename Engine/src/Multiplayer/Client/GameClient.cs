using System;
using System.Net;
using System.Net.Sockets;

class GameClient
{
    static void Main(string[] args)
    {
        Console.WriteLine("Starting GameClient");
        Socket server = Helpers.OpenClientSocket(11111);
        if (server == null)
        {
            Console.WriteLine("Could not create socket to server");
            return;
        }

        Helpers.SendAndReceive(server, "ping");
        Helpers.SendAndReceive(server, "hello");
        Helpers.SendAndReceive(server, "test message");
        
        Console.ReadLine();
    }
}
