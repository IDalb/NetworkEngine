#pragma once

#include "Component/DisplayComponent.h"

#include "Magnum/GL/Buffer.h"
#include "Magnum/GL/Mesh.h"
#include "Magnum/Math/Matrix.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Math/Color.h"
#include <Magnum/Shaders/PhongGL.h>

#include "Utils/InstanceData.h"

namespace GDE
{
	class MeshGroupComponent : public GDE::DisplayComponent
	{
	public:


		static constexpr auto type = "MeshGroup";

		MeshGroupComponent(Entity& owner) : Component(owner) {}
		~MeshGroupComponent() noexcept = default;
		MeshGroupComponent(const MeshGroupComponent&) = default;
		MeshGroupComponent& operator=(const MeshGroupComponent&) = default;
		MeshGroupComponent(MeshGroupComponent&&) = default;
		MeshGroupComponent& operator=(MeshGroupComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void display(Magnum::Shaders::PhongGL& shader, const Timing& dt) override;
		void clean()
		{
			_mesh.reset();
			_buffer.reset();
		}

		Magnum::Containers::Array<InstanceData>& getInstanceData() { return _instanceData; }
	protected:
		std::unique_ptr <Magnum::GL::Buffer> _buffer;
		std::unique_ptr<Magnum::GL::Mesh> _mesh;
		Magnum::Containers::Array<InstanceData> _instanceData;
	};

}
