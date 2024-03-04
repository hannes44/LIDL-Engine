using Engine;
using System;
using System.Numerics;

public class Enemy
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    [SerializableData] public float moveSpeed = 5.0f;

    // Update is called once per frame
    public void Update(float dt)
    {
        Vector3 playerPosition = EngineAPI.GetGameObjectPositionFromTag("Player");
        Vector3 dirToPlayer = playerPosition - gameObject.transform.GetTranslation();
        dirToPlayer = Vector3.Normalize(dirToPlayer);
        gameObject.transform.Translate(dirToPlayer * moveSpeed * dt);
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        Vector3 playerPosition = EngineAPI.GetGameObjectPositionFromTag("Player");
        EngineAPI.Log("Enemy initialized");
        // Randomize the position of the enemy
        Random random = new Random();
        float x = 0;
        float y = 0;
        while (true)
        {
            x = random.Next(-200, 200);
            y = random.Next(-200, 200);
            // Don't spawn the enemy too close to the player
            if (Math.Abs(x - playerPosition.X) > 50 && Math.Abs(y - playerPosition.Z) > 50)
            {
                break;
            }
        }
        gameObject.transform.SetTranslation(new Vector3(x, 0, y));
    }
}
