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
        EngineAPI.PlaySound("AK47_Sound.wav");
    }

    public void OnInput(string action, string actionType)
    {
        if (action == "Fire" && actionType == "ActionDown")
        {
            if (ammoCount <= 0)
            {
                EngineAPI.Log("Out of ammo");
                EngineAPI.PlaySound("Out_Of_Ammo_Sound.wav");
            }
            else
            {
                EngineAPI.PlaySound("AK47_Sound.wav");
                ammoCount--;
            }
        }

        if (action == "Reload")
        {
            EngineAPI.PlaySound("AK47_Reload.wav");
            ammoCount = 30;
        }
    }
}
