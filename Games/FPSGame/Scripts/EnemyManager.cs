using Engine;
using System;
using System.Numerics;

public class EnemyManager
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
        SpawnEnemy();
    }

    private void SpawnEnemy()
    {
        EngineAPI.AddGameObject("Enemy");
    }
}
