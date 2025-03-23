#pragma once
#include "Utils/InstanceData.h"
#include "TypeDef.h"

#include "Magnum/Math/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/Drawable.h"
#include "Magnum/Math/Vector.h"
#include <Corrade/Containers/GrowableArray.h>
namespace GDE
{
    class ColoredDrawable : public Magnum::SceneGraph::Drawable3D
    {
    public:
        ColoredDrawable(Object3D& object, Magnum::Containers::Array<InstanceData>* instanceData, const Magnum::Color3& color, const Magnum::Matrix4& primitiveTransformation, Magnum::SceneGraph::DrawableGroup3D& drawables) : Magnum::SceneGraph::Drawable3D{ object, &drawables }, _instanceData(instanceData), _color{ color }, _primitiveTransformation{ primitiveTransformation } {}
        ~ColoredDrawable();

        void setPrimitiveTransformation(const Magnum::Matrix4& transformation)
        {
            _primitiveTransformation = transformation;
        }
    private:
        void draw(const Magnum::Matrix4& transformation, Magnum::SceneGraph::Camera3D&) override
        {
            const Magnum::Matrix4 t = transformation * _primitiveTransformation;
            arrayAppend(*_instanceData, Magnum::InPlaceInit, t, t.normalMatrix(), _color);
        }

        Magnum::Containers::Array<InstanceData>* _instanceData = nullptr;
        Magnum::Color3 _color;
        Magnum::Matrix4 _primitiveTransformation;
    };
}