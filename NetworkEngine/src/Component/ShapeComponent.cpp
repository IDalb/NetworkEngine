#include "Component/ShapeComponent.h"

namespace GDE
{
	void ShapeComponent::setup(const ComponentDescription& init_value)
	{
		if (init_value.parameters.at("shape").as<std::string>() == "box")
		{
			_shape = BOX;
			_data["width"] = init_value.parameters.at("width").as<float>();
			_data["height"] = init_value.parameters.at("height").as<float>();
			_data["depth"] = init_value.parameters.at("depth").as<float>();
		}
		else
		{
			_shape = SPHERE;
			_data["radius"] = init_value.parameters.at("radius").as<float>();
		}
	}
}