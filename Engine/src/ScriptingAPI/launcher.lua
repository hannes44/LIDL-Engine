package.path = package.path .. ";../../engine/src/ScriptingAPI/C#ToLuaCompiler/Coresystem.lua/?.lua"

require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "GameObject",
        "EngineAPI",
        "test"
    },
    types = {
        "Engine.GameObject",
        "Engine.EngineAPI",
        "Program"
    },
    Main = "Program.Main"
})



--Program.AddComponent()
--Test.Program.Main() -- run main method

--local methodInfo = System.Reflection.Assembly.GetEntryAssembly().getEntryPoint()
--methodInfo:Invoke()
