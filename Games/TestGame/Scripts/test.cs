using Engine;
using System;

public class TestComponent
{
    public static TestComponent create()
    {
        return new TestComponent();
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
        Console.WriteLine("Update TestComponent");
    }

    public void Initialize()
    {
        Console.WriteLine("Initialize TestComponent");
        EngineAPI.Log();
    }

    public int hehe = 10;
}




