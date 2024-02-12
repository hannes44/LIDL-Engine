using System;
using System.Numerics;

namespace Engine
{
    public class Transform
    {
        public Matrix4x4 transformMatrix = Matrix4x4.Identity;

        public void Translate(Vector3 translation)
        {
            transformMatrix *= Matrix4x4.CreateTranslation(translation);
        }

        public void SetTranslation(Vector3 translation)
        {
            transformMatrix.Translation = translation;
        }

        public Vector3 GetTranslation()
        {
            return transformMatrix.Translation;
        }

        // TODO: Gunilla implement rotation

        public void Scale(Vector3 scale)
        {
            transformMatrix *= Matrix4x4.CreateScale(scale);
        }

    }
}



