package.path = package.path .. ";../../engine/src/ScriptingAPI/C#ToLuaCompiler/Coresystem.lua/?.lua"


-- The games script file names will be inserted into the ""Enemy","EnemyManager","Player","TestComponent"," patterns
require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "GameObject",
        "EngineAPI",
        "Transform",
        "Enemy","EnemyManager","Player","TestComponent",
    },
    types = {
        "Engine.GameObject",
        "Engine.EngineAPI",
        "Engine.Transform",
        "Enemy","EnemyManager","Player","TestComponent",
    },
    Main = "Program.Main"
})
