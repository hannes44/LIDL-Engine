using Engine;
using System;
using System.Numerics;

public class Enemy
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();


    // Update is called once per frame
    public void Update()
    {

    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        // Randomize the position of the enemy
        Random random = new Random();
        int x = random.Next(0, 100);
        int y = random.Next(0, 100);
        gameObject.transform.SetTranslation(new Vector3(x, 0, y));
    }
}
