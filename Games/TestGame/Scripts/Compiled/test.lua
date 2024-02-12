-- Generated by CSharp.lua Compiler
local System = System
local SystemNumerics = System.Numerics
local Engine
System.import(function (out)
  Engine = out.Engine
end)
System.namespace("", function (namespace)
  namespace.class("TestComponent", function (namespace)
    local Update, Initialize, __ctor__
    __ctor__ = function (this)
      this.gameObject = Engine.GameObject()
    end
    Update = function (this)
      this.gameObject.transform:Translate(SystemNumerics.Vector3(0.1, 0, 0))
    end
    Initialize = function (this)
      System.Console.WriteLine("Initialize TestComponent")
      System.Console.WriteLine(this.gameObject.name)
      System.Console.WriteLine(this.Id)

      this.gameObject.transform:SetTranslation(SystemNumerics.Vector3(1, 10, 1))

      System.Console.WriteLine(this.gameObject.transform.transformMatrix)
      Engine.EngineAPI.Log()
    end
    return {
      Update = Update,
      Initialize = Initialize,
      __ctor__ = __ctor__
    }
  end)
end)
