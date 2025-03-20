#pragma once
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/Drawable.h"
#include "Magnum/Math/Vector.h"
#include <Corrade/Containers/GrowableArray.h>

#include "Utils/InstanceData.h"
#include "TypeDef.h"
namespace GDE
{
    class ColoredDrawable : public Magnum::SceneGraph::Drawable3D
    {
    public:
        explicit ColoredDrawable(Object3D& object, Magnum::Containers::Array<InstanceData>& instanceData, const Magnum::Color3& color, const Magnum::Matrix4& primitiveTransformation, Magnum::SceneGraph::DrawableGroup3D& drawables) : Magnum::SceneGraph::Drawable3D{ object, &drawables }, _instanceData(instanceData), _color{ color }, _primitiveTransformation{ primitiveTransformation } {}

    private:
        void draw(const Magnum::Matrix4& transformation, Magnum::SceneGraph::Camera3D&) override
        {
            const Magnum::Matrix4 t = transformation * _primitiveTransformation;
            arrayAppend(_instanceData, Magnum::InPlaceInit, t, t.normalMatrix(), _color);
        }

        Magnum::Containers::Array<InstanceData>& _instanceData;
        Magnum::Color3 _color;
        Magnum::Matrix4 _primitiveTransformation;
    };
}