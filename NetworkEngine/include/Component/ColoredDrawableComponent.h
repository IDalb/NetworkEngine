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
		~ColoredDrawableComponent() {}
		ColoredDrawableComponent(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent& operator=(const ColoredDrawableComponent&) = default;
		ColoredDrawableComponent(ColoredDrawableComponent&&) = default;
		ColoredDrawableComponent& operator=(ColoredDrawableComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void resolve();
		void updateSize();

		void setValue(std::string_view variable, float value) override;
		const Magnum::Color3& getColor() const { return _object->getColor(); }

    private:
		ColoredDrawable* _object = nullptr;
		std::string _meshGroup = "";
		Magnum::Vector3 _color;

		enum Color
		{
			R,G,B
		};
		inline static std::map<std::string, Color> FIELD_MAP = { {"color_r", R}, {"color_b", B}, {"color_g", G} };
    };
}
