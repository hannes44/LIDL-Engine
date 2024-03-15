using Engine;
using System;
using System.Numerics;

public class TemplateComponent
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();


    // Update is called once per frame
    public void Update(float dt)
    {
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
    }

    public void OnInput(string action, string actionType)
    {
    }
}
