-- Generated by CSharp.lua Compiler
local System = System
local Engine
System.import(function (out)
  Engine = out.Engine
end)
System.namespace("", function (namespace)
  namespace.class("TestComponent2", function (namespace)
    local create, Main, AddComponent, Update, Initialize, class
    create = function ()
      return class()
    end
    Main = function (this)
      System.Console.WriteLine("hello lua!")
      AddComponent(this)
    end
    AddComponent = function (this)
      System.Console.WriteLine("Adding Component")
      System.Console.WriteLine("HEHE KEBAB22")

      local gameObject = Engine.GameObject()
      System.Console.WriteLine(gameObject.name)
    end
    Update = function (this)
      System.Console.WriteLine("Update TestComponent2")
    end
    Initialize = function (this)
      System.Console.WriteLine("Initialize TestComponent2")
      --EngineAPI.Log();
    end
    class = {
      create = create,
      Main = Main,
      AddComponent = AddComponent,
      Update = Update,
      Initialize = Initialize,
      hehe = 10
    }
    return class
  end)
end)