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

    public int enemiesToSpawn = 10;



    // Update is called once per frame
    public void Update()
    {

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
            EngineAPI.SpawnClonedGameObjectFromTag("EnemyPrefab");
        }
    }

    private void NextRound()
    {
        currentRound++;

        // Increasing the number of enemies to spawn each round
        enemiesToSpawn += 5;
    }
}
