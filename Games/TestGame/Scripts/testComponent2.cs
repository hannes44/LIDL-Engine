using Engine;
using System;

public class TestComponent2
{
    public static TestComponent2 create()
    {
        return new TestComponent2();
    }

    public void Main()
    {
        Console.WriteLine("hello lua!");
        AddComponent();
    }

    public void AddComponent()
    {
        Console.WriteLine("Adding Component");
        Console.WriteLine("HEHE KEBAB22");

        GameObject gameObject = new GameObject();
        Console.WriteLine(gameObject.name);
    }

    public void Update()
    {
        Console.WriteLine("Update TestComponent2");
    }

    public void Initialize()
    {
        Console.WriteLine("Initialize TestComponent2");
        //EngineAPI.Log();
    }

    public int hehe = 10;
}




