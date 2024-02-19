using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class GameServer
{
    static void Main()
    {
        Console.WriteLine("Starting GameServer");
        Socket client = Helpers.OpenServerSocket(11111);

        try
        {
            while (true)
            {

                Console.WriteLine("Waiting to receive message... ");

                string data = Helpers.ReceiveMessage(client);

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
