using Engine;
using System;

public class TestComponent
{
    public string Id;

    public GameObject gameObject = new GameObject();

    public void Update()
    {
        Console.WriteLine("Update TestComponent");
    }

    public void Initialize()
    {

        Console.WriteLine("Initialize TestComponent");
        Console.WriteLine(gameObject.name);
        Console.WriteLine(Id);
        Console.WriteLine(gameObject.transform.position.X);

        EngineAPI.Log();

    }
}
