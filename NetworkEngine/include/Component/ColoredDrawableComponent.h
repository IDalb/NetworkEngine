#pragma once
#include "Component/Component.h"

#include "Magnum/Math/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/Drawable.h"
#include "Magnum/Math/Vector.h"

#include "Utils/ColoredDrawable.h"

namespace GDE
{
    class ColoredDrawableComponent : public Component {
    public:
		static constexpr auto type = "ColoredDrawable";

		ColoredDrawableComponent(Entity& owner) : Component(owner) {}
		ColoredDrawableComponent(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent& operator=(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent(ColoredDrawableComponent&&) = default;
		ColoredDrawableComponent& operator=(ColoredDrawableComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void resolve();

    private:
		ColoredDrawable* _object;
		std::string _meshGroup = "";
		Magnum::Vector3 _color;
    };
}
