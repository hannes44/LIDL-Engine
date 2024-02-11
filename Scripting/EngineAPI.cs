using System;

namespace Scripting
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
    }
}
