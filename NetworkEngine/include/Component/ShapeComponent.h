#pragma once
#include "Component/Component.h"
#include "std.h"

namespace GDE
{
	class ShapeComponent : public Component
	{
	public:
		enum Shape
		{
			BOX, SPHERE
		};

		static constexpr auto type = "Shape";

		ShapeComponent(Entity& owner) : Component(owner) {}
		ShapeComponent(const ShapeComponent&) = default;
		ShapeComponent& operator=(const ShapeComponent&) = default;
		ShapeComponent(ShapeComponent&&) = default;
		ShapeComponent& operator=(ShapeComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;

		Shape _shape{};

		float getData(const std::string& data) const { return _data.contains(data) ? _data.at(data) : 0; }

	protected:
		std::map<std::string, float> _data{};
	};

}
