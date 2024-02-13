using System;

namespace Engine
{

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

        public static GameObject GetComponentGameObject()
        {
            GameObject gameObject = new GameObject();
            return gameObject;
        }
    }
}
