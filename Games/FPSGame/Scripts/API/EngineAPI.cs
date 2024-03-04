using System;
using System.Numerics;

namespace Engine
{

    // Fields marked with this attribute will be serialized and editable in the editor
    public class SerializableData : Attribute { }

    // The EngineAPI, responsible for abstracting away the c++ binding away from the user
    // Since the c++ bindings only work with lua, we compile the C# files to lua
    // We encode the bindings in this file under the pattern "//(c++_API) bindingFunctionName();"
    // All binding functions have 2 underscores before and after the function name to avoid the user
    // from accidentally naming their own functions the same as the binding functions
    static class EngineAPI
    {
        public static void AddGameObject(string name)
        {
            //(c++_API) __addGameObject__(name);
        }

        public static void Log(string message)
        {
            //(c++_API) __log__(message);
        }

        // Clones a game object from a game object with the given tag and spawns it
        public static void SpawnClonedGameObjectFromTag(string tag)
        {
            //(c++_API) __spawnClonedGameObjectFromTag__(tag);
        }

        public static void PlaySound(string soundName)
        {
            //(c++_API) __playSound__(soundName);
        }

        public static string GetIdOfGameObjectHitByRay(float originX, float originY, float originZ, float rayX, float rayY, float rayZ)
        {
            //(c++_API) id = __getIdOfGameObjectHitByRay__(originX, originY, originZ, rayX, rayY, rayZ);
            //(c++_API) if (id ~= "") then return id; 
            //(c++_API) else 
            return "";
            //(c++_API) end
        }

        public static Vector3 GetGameObjectPositionFromTag(string tag)
        {
            Vector3 position = new Vector3(0, 0, 0);
            //(c++_API) x, y, z = __getGameObjectPositionFromTag__(tag);
            //(c++_API) if (true) then return SystemNumerics.Vector3(x, y, z);
            //(c++_API) else 
            return position;
            //(c++_API) end
        }

        public static string getTagOfGameObject(string id)
        {
            //(c++_API) tag = __getTagOfGameObject__(id);
            //(c++_API) if (tag ~= "") then return tag; 
            //(c++_API) else 
            return "";
            //(c++_API) end
        }

        public static int getNumberOfGameObjectsWithTag(string tag)
        {
            //(c++_API) number = __getNumberOfGameObjectsWithTag__(tag);
            //(c++_API) if (number ~= 0) then return number; 
            //(c++_API) else 
            return 0;
            //(c++_API) end
        }

        public static void DeleteGameObjectFromId(string id)
        {
            //(c++_API) __deleteGameObjectFromId__(id);
        }

        public static GameObject GetComponentGameObject()
        {
            GameObject gameObject = new GameObject();
            return gameObject;
        }
    }
}
