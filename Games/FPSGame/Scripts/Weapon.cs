using Engine;
using System;
using System.Numerics;

public class Weapon
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    [SerializableData] public int ammoCount = 30;


    // Update is called once per frame
    public void Update()
    {

    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {

    }
}
