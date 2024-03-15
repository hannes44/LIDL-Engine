using Engine;
using System;
using System.Numerics;

public class Player
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    [SerializableData] public int counter = 0;
    private bool started = false;
    private float timeSinceMoved = 0;


    // Update is called once per frame
    public void Update(float dt)
    {
        timeSinceMoved += dt;
        if (started && timeSinceMoved > 2.0f) {
            Random random = new Random();
            gameObject.transform.SetTranslation(new Vector3(random.Next(-2, 2), random.Next(-2, 2), random.Next(-2, 2)));
            timeSinceMoved = 0;
        }

        EngineAPI.DrawText(0.5f, 0.5f, 2.0f, "Counter: " + counter, 0, 1, 0, 1);
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        Random random = new Random();
        gameObject.transform.SetTranslation(new Vector3(random.Next(-2, 2), random.Next(-2, 2), random.Next(-2, 2)));
    }

    public void OnInput(string action, string actionType)
    {
        if (action == "SpawnObject" && actionType == "ActionDown")
            counter++;

        if (action == "Start" && actionType == "ActionDown")
            started = true;
    }
}
