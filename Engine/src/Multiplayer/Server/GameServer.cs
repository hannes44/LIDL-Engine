using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class GameServer
{
    static void Main()
    {
        Console.WriteLine("Starting GameServer");
        
        try
        {
            Socket client = Helpers.OpenServerSocket(11111);
            
            Console.WriteLine("Waiting for a connection...");
            client = client.Accept();
            
            while (true)
            {

                Console.WriteLine("Waiting to receive message... ");

                string data = Helpers.ReceiveMessage(client);

                if (data == null || data == "exit") {
                    Console.WriteLine("Closing connection...");
                    Helpers.CloseSocket(client);
                    break;
                }

                string response = null;
                response = data switch
                {
                    "ping" => "pong",
                    "hello" => "world",
                    _ => "Unknown command",
                };
                
                Helpers.SendMessage(client, response);
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
        
        Console.ReadLine();
    }
}
