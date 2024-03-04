using Engine;
using System;
using System.Numerics;

public enum WeaponType
{
    Knife,
    Glock,
    AK47
}

public class Player
{
    // The unique identifier for this component
    public string Id;
    // The gameObject this component is attached to
    public GameObject gameObject = new GameObject();

    const int maxAmmoAK = 30;

    [SerializableData] public int ammoCountAK = maxAmmoAK;

    const int maxAmmoGlock = 20;

    [SerializableData] public int ammoCountGlock = maxAmmoGlock;

    WeaponType selectedWeapon = WeaponType.AK47;

    // Update is called once per frame
    public void Update()
    {
        if (selectedWeapon != WeaponType.Knife)
            EngineAPI.DrawText(0.9f, 0.9f, "Ammo: " + GetSelectedWeaponAmmoCount() + "/" + GetSelectedWeaponMaxAmmo(), 1, 1, 1, 1);
    }

    // Initialize is called once the component is added to the game
    public void Initialize()
    {
        selectedWeapon = WeaponType.AK47;
        HideAllWeapons();
        EngineAPI.SetGameObjectMeshVisibilityFromTag("AK47", true);
    }

    private void HideAllWeapons()
    {
        EngineAPI.SetGameObjectMeshVisibilityFromTag("Knife", false);
        EngineAPI.SetGameObjectMeshVisibilityFromTag("AK47", false);
        EngineAPI.SetGameObjectMeshVisibilityFromTag("Pistol", false);
    }

    private void PlayOutOfAmmoSound()
    {
        if (selectedWeapon == WeaponType.Knife)
        {
            return;
        }
        else if (selectedWeapon == WeaponType.Glock)
        {
            EngineAPI.PlaySound("Out_Of_Ammo_Sound.wav");
        }
        else if (selectedWeapon == WeaponType.AK47)
        {
            EngineAPI.PlaySound("Out_Of_Ammo_Sound.wav");
        }
    }

    private int GetSelectedWeaponAmmoCount()
    {
        if (selectedWeapon == WeaponType.Knife)
        {
            return -1;
        }
        else if (selectedWeapon == WeaponType.Glock)
        {
            return ammoCountGlock;
        }
        else if (selectedWeapon == WeaponType.AK47)
        {
            return ammoCountAK;
        }
        return -1;
    }

    private int GetSelectedWeaponMaxAmmo()
    {
        if (selectedWeapon == WeaponType.Knife)
        {
            return -1;
        }
        else if (selectedWeapon == WeaponType.Glock)
        {
            return maxAmmoGlock;
        }
        else if (selectedWeapon == WeaponType.AK47)
        {
            return maxAmmoAK;
        }
        return -1;
    }

    private void PlayFireWeaponSound()
    {
        if (selectedWeapon == WeaponType.Knife)
        {
            return;
        }
        else if (selectedWeapon == WeaponType.Glock)
        {
            EngineAPI.PlaySound("Glock_Sound.wav");
        }
        else if (selectedWeapon == WeaponType.AK47)
        {
            EngineAPI.PlaySound("AK47_Sound.wav");
        }
    }

    private void ReloadSelectedWeapon()
    {
        if (selectedWeapon == WeaponType.Knife)
        {
            return;
        }
        else if (selectedWeapon == WeaponType.Glock)
        {
            ammoCountGlock = maxAmmoGlock;
            EngineAPI.PlaySound("Glock_Reload.wav");
        }
        else if (selectedWeapon == WeaponType.AK47)
        {
            EngineAPI.PlaySound("AK47_Reload.wav");
            ammoCountAK = maxAmmoAK;
        }
    }



    private void FireWeapon()
    {
        int ammoCount = GetSelectedWeaponAmmoCount();

        if (ammoCount <= 0)
        {
            PlayOutOfAmmoSound();
        }
        else
        {
            PlayFireWeaponSound();


            if (selectedWeapon == WeaponType.Knife)
            {
                return;
            }
            else if (selectedWeapon == WeaponType.Glock)
            {
                ammoCountGlock--;
            }
            else if (selectedWeapon == WeaponType.AK47)
            {
                ammoCountAK--;
            }

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

    public void OnInput(string action, string actionType)
    {
        if (action == "SwitchToKnife" && actionType == "ActionDown")
        {
            selectedWeapon = WeaponType.Knife;
            HideAllWeapons();
            EngineAPI.SetGameObjectMeshVisibilityFromTag("Knife", true);
        }

        if (action == "SwitchToRifle" && actionType == "ActionDown")
        {
            selectedWeapon = WeaponType.AK47;
            HideAllWeapons();
            EngineAPI.SetGameObjectMeshVisibilityFromTag("AK47", true);
        }

        if (action == "SwitchToPistol" && actionType == "ActionDown")
        {
            selectedWeapon = WeaponType.Glock;
            HideAllWeapons();
            EngineAPI.SetGameObjectMeshVisibilityFromTag("Pistol", true);
        }

        if (action == "Fire" && actionType == "ActionDown")
        {
            FireWeapon();
        }

        if (action == "Reload")
        {
            ReloadSelectedWeapon();
        }
    }
}
