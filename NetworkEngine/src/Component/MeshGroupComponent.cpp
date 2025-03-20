#include "Component/MeshGroupComponent.h"

#include "Entity.h"
#include "Scene.h"

#include "Component/TransformComponent.h"
#include "Component/MeshGroupComponent.h"


#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/UVSphere.h>
#include <Magnum/Trade/MeshData.h>

#include <Corrade/Containers/GrowableArray.h>

namespace GDE
{
	void MeshGroupComponent::setup(const ComponentDescription& init_value)
	{
		if (init_value.parameters.at("shape").as<std::string>() == "box")
		{
			_mesh = Magnum::MeshTools::compile(Magnum::Primitives::cubeSolid());
		}
		if (init_value.parameters.at("shape").as<std::string>() == "sphere")
		{
			_mesh = Magnum::MeshTools::compile(Magnum::Primitives::uvSphereSolid(16, 32));
		}
		_buffer = Magnum::GL::Buffer{};
		_mesh.addVertexBufferInstanced(_buffer, 1, 0,
			Magnum::Shaders::PhongGL::TransformationMatrix{},
			Magnum::Shaders::PhongGL::NormalMatrix{},
			Magnum::Shaders::PhongGL::Color3{});
	}

	void MeshGroupComponent::display(Magnum::Shaders::PhongGL& shader, const Timing& dt)
	{
		arrayResize(_instanceData, 0);
		_buffer.setData(_instanceData, Magnum::GL::BufferUsage::DynamicDraw);
		_mesh.setInstanceCount(_instanceData.size());
		shader.draw(_mesh);
	}
}