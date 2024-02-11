package.path = package.path .. ";../../C#ToLuaCompiler/Coresystem.lua/?.lua"

require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "Library",
        "EngineAPI",
        "test"
    },
    types = {
        "Potatis.Library",
        "Scripting.EngineAPI",
        "Program"
    },
    Main = "Program.Main"
})



--Program.AddComponent()
--Test.Program.Main() -- run main method

--local methodInfo = System.Reflection.Assembly.GetEntryAssembly().getEntryPoint()
--methodInfo:Invoke()
