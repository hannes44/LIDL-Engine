﻿using System;

namespace Engine
{
    static class EngineAPI
    {
        public static void AddGameObject()
        {
            System.Console.WriteLine("AddGameObject()");
        }

        public static void Log()
        {
            Console.WriteLine("Log()");
            AddGameObject();
        }

        public static GameObject GetComponentGameObject()
        {
            GameObject gameObject = new GameObject();
            return gameObject;
        }
    }
}
