using Potatis;
using Scripting;
using System;

public static class Program
{
    public static void Main()
    {
        Console.WriteLine("hello lua!");
        AddComponent();
    }

    public static void AddComponent()
    {
        Console.WriteLine("Adding Component");
        Console.WriteLine("HEHE KEBAB");

        Console.WriteLine(Library.kebabrulle);
        Library.yes();

        EngineAPI.AddGameObject();
        // Console.WriteLine(Library.getInt());
    }

    public static int hehe = 10;
}




