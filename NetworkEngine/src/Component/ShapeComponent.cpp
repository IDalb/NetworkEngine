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
			_globalSize = Magnum::Vector3(_data["sx"], _data["sy"], _data["sz"]);
		}
		else
		{
			if (init_value.parameters.at("shape").as<std::string>() == "sphere")
			{
				_shape = SPHERE;
				_data["radius"] = init_value.parameters.at("radius").as<float>();
				_globalSize = Magnum::Vector3(_data["radius"], _data["radius"], _data["radius"]);
			}
		}
	}
	void ShapeComponent::setValue(std::string_view variable, float value)
	{
		std::string strVariable(variable);
		switch (FIELD_MAP.at(strVariable))
		{
		case GDE::ShapeComponent::S_x:
			_data["sx"] = value;
			_globalSize = Magnum::Vector3(_data["sx"], _data["sy"], _data["sz"]);
			break;
		case GDE::ShapeComponent::S_y:
			_data["sy"] = value;
			_globalSize = Magnum::Vector3(_data["sx"], _data["sy"], _data["sz"]);
			break;
		case GDE::ShapeComponent::S_z:
			_data["sz"] = value;
			_globalSize = Magnum::Vector3(_data["sx"], _data["sy"], _data["sz"]);
			break;
		case GDE::ShapeComponent::Radius:
			_data["radius"] = value;
			_globalSize = Magnum::Vector3(_data["radius"], _data["radius"], _data["radius"]);
			break;
		}
		for (auto& pair : _callbacks)
		{
			pair.first(pair.second, strVariable);
		}
	}
}