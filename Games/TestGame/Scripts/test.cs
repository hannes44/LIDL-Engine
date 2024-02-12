using Engine;
using System;
using System.Numerics;

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

        gameObject.transform.transformMatrix.Translation = new Vector3(1, 10, 1);
        Console.WriteLine(gameObject.transform.transformMatrix);
        EngineAPI.Log();

    }
}
