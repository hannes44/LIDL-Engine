-- Generated by CSharp.lua Compiler
local System = System
local Potatis
local Scripting
System.import(function (out)
  Potatis = out.Potatis
  Scripting = out.Scripting
end)
System.namespace("", function (namespace)
  namespace.class("Program", function (namespace)
    local Main, AddComponent
    Main = function ()
      System.Console.WriteLine("hello lua!")
      AddComponent()
    end
    AddComponent = function ()
      System.Console.WriteLine("Adding Component")
      System.Console.WriteLine("HEHE KEBAB")

      System.Console.WriteLine(Potatis.Library.kebabrulle)
      Potatis.Library.yes()

      Scripting.EngineAPI.AddGameObject()
      -- Console.WriteLine(Library.getInt());
    end
    return {
      Main = Main,
      AddComponent = AddComponent,
      hehe = 10
    }
  end)
end)
