using Engine;
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
        Console.WriteLine("HEHE KEBAB2222");

        GameObject gameObject = new GameObject();
        Console.WriteLine(gameObject.name);


        // EngineAPI.AddGameObject();
        // Console.WriteLine(Library.getInt());
    }

    public static int hehe = 10;
}




