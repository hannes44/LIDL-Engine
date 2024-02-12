using Engine;
using System;
using System.Numerics;

public static class Program
{


    public static void Main()
    {
        TestComponent testComponent = new TestComponent();
        testComponent.Initialize();
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

public class TestComponent
{
    public Matrix4x4 vector3;

    public Vector3 vector = Vector3.One;

    int hejsan = 11;

    public TestComponent()
    {
        Console.WriteLine("TestComponent");
        hejsan = 12;
    }
    public void Update()
    {
        Console.WriteLine("Update TestComponent");
    }

    public void Initialize()
    {

        Console.WriteLine("Initialize TestComponent");
        Console.WriteLine(hejsan);
        Update();

        EngineAPI.Log();

    }
}

