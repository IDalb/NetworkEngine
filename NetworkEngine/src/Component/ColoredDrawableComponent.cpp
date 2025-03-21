#include "Component/ColoredDrawableComponent.h"
#include "Entity.h"
#include "Scene.h"

#include "Component/TransformComponent.h"
#include "Component/ShapeComponent.h"
#include "Component/MeshGroupComponent.h"

#include "System/DisplaySystem.h"

namespace GDE
{
	void ColoredDrawableComponent::setup(const ComponentDescription& init_value)
	{
		_meshGroup = init_value.parameters.at("mesh_group").as<std::string>();
		if (init_value.parameters.contains("color"))
		{
			_color = Magnum::Vector3(init_value.parameters.at("color")[0].as<float>(),
				init_value.parameters.at("color")[1].as<float>(),
				init_value.parameters.at("color")[2].as<float>());
		}
	}
	void ColoredDrawableComponent::resolve()
	{
		_object = new ColoredDrawable(
			owner().getComponent<TransformComponent>()->getTransform(),
			&Scene::findEntityWithTag(_meshGroup)->getComponent<MeshGroupComponent>()->getInstanceData(),
			Magnum::Color3(_color),
			Magnum::Matrix4::scaling(owner().getComponent<ShapeComponent>()->getGlobalSize()),
			DisplaySystem::getInstance().getDrawable());
		
	}
}
