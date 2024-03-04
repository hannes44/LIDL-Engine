using Engine;
using System;
using System.Numerics;

public class Weapon
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    const int maxAmmo = 30;

    [SerializableData] public int ammoCount = maxAmmo;


    // Update is called once per frame
    public void Update(float dt)
    {
        EngineAPI.DrawText(0.9f, 0.9f, "Ammo: " + ammoCount + "/" + maxAmmo, 1, 1, 1, 1);
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        EngineAPI.PlaySound("AK47_Sound.wav");
    }
    //
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
                //

                // Fire ray in the forward direction of the weapon
                Vector3 weaponPosition = gameObject.transform.GetTranslation();
                Vector3 weaponForward = gameObject.transform.GetForward();
                Console.WriteLine("Weapon position: " + weaponPosition);
                Console.WriteLine("Weapon forward: " + weaponForward);
                string hitId = EngineAPI.GetIdOfGameObjectHitByRay(weaponPosition.X, weaponPosition.Y, weaponPosition.Z, weaponForward.X, weaponForward.Y, weaponForward.Z);
                if (hitId != "")
                {
                    EngineAPI.Log("Hit object with id: " + hitId);
                    string tag = EngineAPI.getTagOfGameObject(hitId);
                    if (tag == "Enemy")
                    {
                        EngineAPI.DeleteGameObjectFromId(hitId);
                    }
                }
            }
        }

        if (action == "Reload")
        {
            EngineAPI.PlaySound("AK47_Reload.wav");
            ammoCount = 30;
        }
    }
}
