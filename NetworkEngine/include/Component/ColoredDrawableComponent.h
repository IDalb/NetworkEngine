#pragma once
#include "Component/Component.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/Drawable.h"
#include "Magnum/Math/Vector.h"

namespace GDE
{
    class ColoredDrawableComponent :  GDE::Component {
    private:
		static constexpr auto type = "ColoredDrawable";

		ColoredDrawableComponent(Entity& owner) : Component(owner) {}
		ColoredDrawableComponent(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent& operator=(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent(ColoredDrawableComponent&&) = default;
		ColoredDrawableComponent& operator=(ColoredDrawableComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void resolve();

    public:
		std::unique_ptr<Magnum::SceneGraph::Drawable3D> _object;
		std::string _meshGroup = "";
		Magnum::Vector3 _color;
    };
}
