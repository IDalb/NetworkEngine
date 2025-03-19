#pragma once
#include "DisplayComponent.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/Drawable.h"

using namespace Magnum;

namespace GDE
{
    class ColoredDrawableComponent : public GDE::DisplayComponent, public Magnum::SceneGraph::Drawable3D {
    private:
        Color3 _color;
        void draw(const Magnum::Matrix4& transformation, SceneGraph::Camera3D&) override {
            const Matrix4 t = transformation * _primitiveTransformation;
            arrayAppend(_instanceData, InPlaceInit, t, t.normalMatrix(), _color);
        }

        Containers::Array<InstanceData>& _instanceData;
        Matrix4 _primitiveTransformation;

    public:
        explicit ColoredDrawableComponent(Object3D& object, Containers::Array<InstanceData>& instanceData, const Color3& color, const Matrix4& primitiveTransformation, SceneGraph::DrawableGroup3D& drawables):
        SceneGraph::Drawable3D(object, &drawables), _instanceData(instanceData), _color(color), _primitiveTransformation(primitiveTransformation) {}
    };
}
