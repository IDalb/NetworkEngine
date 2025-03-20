#include "Component/ShapeComponent.h"

namespace GDE
{
	void ShapeComponent::setup(const ComponentDescription& init_value)
	{
		if (init_value.parameters.at("shape").as<std::string>() == "box")
		{
			_shape = BOX;
			_data["sx"] = init_value.parameters.at("size")[0].as<float>();
			_data["sy"] = init_value.parameters.at("size")[1].as<float>();
			_data["sz"] = init_value.parameters.at("size")[2].as<float>();
		}
		else
		{
			if (init_value.parameters.at("shape").as<std::string>() == "sphere")
			{
				_shape = SPHERE;
				_data["radius"] = init_value.parameters.at("radius").as<float>();
			}
		}
	}
}