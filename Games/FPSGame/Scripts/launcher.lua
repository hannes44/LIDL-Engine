package.path = package.path .. ";../../engine/src/ScriptingAPI/C#ToLuaCompiler/Coresystem.lua/?.lua"


-- The games script file names will be inserted into the ""EnemyManager","TestComponent"," patterns
require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "GameObject",
        "EngineAPI",
        "Transform",
        "EnemyManager","TestComponent",
    },
    types = {
        "Engine.GameObject",
        "Engine.EngineAPI",
        "Engine.Transform",
        "EnemyManager","TestComponent",
    },
    Main = "Program.Main"
})
