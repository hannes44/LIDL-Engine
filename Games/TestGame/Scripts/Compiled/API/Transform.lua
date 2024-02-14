-- Generated by CSharp.lua Compiler
local System = System
local SystemNumerics = System.Numerics
System.namespace("Engine", function (namespace)
  namespace.class("Transform", function (namespace)
    local Translate, SetTranslation, GetTranslation, Scale, __ctor__
    __ctor__ = function (this)
      this.transformMatrix = SystemNumerics.Matrix4x4.getIdentity()
    end
    Translate = function (this, translation)
      this.transformMatrix = SystemNumerics.Matrix4x4.op_Multiply(this.transformMatrix, SystemNumerics.Matrix4x4.CreateTranslation(translation))
    end
    SetTranslation = function (this, translation)
      this.transformMatrix:setTranslation(translation)
    end
    GetTranslation = function (this)
      return this.transformMatrix:getTranslation()
    end
    Scale = function (this, scale)
      this.transformMatrix = SystemNumerics.Matrix4x4.op_Multiply(this.transformMatrix, SystemNumerics.Matrix4x4.CreateScale(scale))
    end
    return {
      Translate = Translate,
      SetTranslation = SetTranslation,
      GetTranslation = GetTranslation,
      Scale = Scale,
      __ctor__ = __ctor__
    }
  end)
end)