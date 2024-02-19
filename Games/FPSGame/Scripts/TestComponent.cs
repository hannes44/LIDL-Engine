using Engine;
using System;
using System.Numerics;

public class TestComponent
{
    public string Id;

    public GameObject gameObject = new GameObject();

    [SerializableData] public float test = 10.0f;

    [SerializableData] public bool countNegative = false;

    public void Update()
    {

        gameObject.transform.Translate(new Vector3(0.1f, 0, 0));
        test += countNegative ? -1.0f : 1.0f;
    }

    public void Initialize()
    {

        Console.WriteLine("Initialize TestComponent");
        Console.WriteLine(gameObject.name);
        Console.WriteLine(Id);

        gameObject.transform.SetTranslation(new Vector3(1, 10, 1));

        Console.WriteLine(gameObject.transform.transformMatrix);
        EngineAPI.Log("hejsan potatismos");

        EngineAPI.AddGameObject("GIAGACHAD CSHARP GAMEOBJECT");

    }

    public void OnInput()
    {
        Console.WriteLine("OnInput TestComponent");
        EngineAPI.Log("ONINPUT");
    }
}
