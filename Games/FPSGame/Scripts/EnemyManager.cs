using Engine;
using System;
using System.Numerics;

public class EnemyManager
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    [SerializableData] public int currentRound = 1;

    public GameObject enemyPrefab = new GameObject();

    public int enemiesToSpawn = 1;



    // Update is called once per frame
    public void Update(float dt)
    {
        EngineAPI.DrawText(0.0f, 0.0f, "Round: " + currentRound, 1, 1, 1, 1);
        if (EngineAPI.getNumberOfGameObjectsWithTag("Enemy") == 1)
        {
            NextRound();
            SpawnEnemy();
        }
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        SpawnEnemy();
    }

    private void SpawnEnemy()
    {
        for (int i = 0; i < enemiesToSpawn; i++)
        {
            EngineAPI.Log("Spawning enemy");
            EngineAPI.SpawnClonedGameObjectFromTag("Enemy");
        }
    }

    private void NextRound()
    {
        currentRound++;

        // Increasing the number of enemies to spawn each round
        enemiesToSpawn += 1;
    }
}
