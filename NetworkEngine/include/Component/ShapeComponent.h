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
		const Magnum::Vector3& getGlobalSize() const { return _globalSize; }

		void setValue(std::string_view variable, float value) override;
		void setSizeChangeCallback(std::pair<std::function<void(GDE::Component*, const std::string&)>, GDE::Component*> callback)
		{
			_callbacks.push_back(callback);
		}
	protected:
		std::map<std::string, float> _data{};
		Magnum::Vector3 _globalSize{};
		std::vector<std::pair<std::function<void(GDE::Component*, const std::string&)>, GDE::Component*>> _callbacks;

		enum Field
		{
			S_x, S_y, S_z, Radius
		};

		inline static std::map<std::string, Field> FIELD_MAP = { {"radius", Radius} , {"size_x", S_x}, {"size_y", S_y}, {"size_z", S_z} };

	};

}
