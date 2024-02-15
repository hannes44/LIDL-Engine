package.path = package.path .. ";../../engine/src/ScriptingAPI/C#ToLuaCompiler/Coresystem.lua/?.lua"


-- The games script file names will be inserted into the NewComponent.cs,TestComponent.cs, sections
require("All")() -- coresystem.lua/All.lua
--require("out.manifest")("out")
System.init({
    path = "",
    files = {
        "GameObject",
        "EngineAPI",
        "Transform",
        --FileNames--
    },
    types = {
        "Engine.GameObject",
        "Engine.EngineAPI",
        "Engine.Transform",
        --FileNames--
    },
    Main = "Program.Main"
})
