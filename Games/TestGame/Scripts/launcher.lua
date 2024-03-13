package.path = package.path .. ";../../../engine/src/ScriptingAPI/C#ToLuaCompiler/Coresystem.lua/?.lua"


-- The games script file names will be inserted into the ""TestComponent"," patterns
require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "GameObject",
        "EngineAPI",
        "Transform",
        "TestComponent",
    },
    types = {
        "Engine.GameObject",
        "Engine.EngineAPI",
        "Engine.Transform",
        "TestComponent",
    },
    Main = "Program.Main"
})
