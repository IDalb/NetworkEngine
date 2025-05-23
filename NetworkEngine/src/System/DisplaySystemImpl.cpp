#include "System/DisplaySystemImpl.h"
#include "System/PhysicsSystem.h"
#include "Component/DisplayComponent.h"
#include "Component/MeshGroupComponent.h"
#include "Component/CameraComponent.h"

#include "Entity.h"
#include "Scene.h"
#include "GlobalConstants.h"
#include "System/GuiSystem.h"

#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Integration.h>
#include <Magnum/Math/Color.h>

using namespace Magnum::Math::Literals;

namespace GDE
{
	void DisplaySystemImpl::setup()
	{
		_shader = std::make_unique<Magnum::Shaders::PhongGL>(Magnum::Shaders::PhongGL::Configuration{}
			   .setFlags(Magnum::Shaders::PhongGL::Flag::VertexColor |
						 Magnum::Shaders::PhongGL::Flag::InstancedTransformation));
		_shader->setAmbientColor(0x111111_rgbf)
			.setSpecularColor(0x330000_rgbf)
			.setLightPositions({ {0, 0, 15.0f, 0.0f} });

		Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
		Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
		Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::PolygonOffsetFill);
		Magnum::GL::Renderer::setPolygonOffset(2.0f, 0.5f);
	}

	void DisplaySystemImpl::clean()
	{
		_shader.reset();
		for (auto& component : _displayComponents)
		{
			if (auto meshComp = dynamic_cast<MeshGroupComponent*>(component))
			{
				meshComp->clean();
			}
		}
	}

	void DisplaySystemImpl::iterate(const Timing& dt)
	{
		if(_enable)
		{
			renderScene(dt);
			renderUi();
		}
	}

	void DisplaySystemImpl::renderUi()
	{
		if (GuiSystem::_exist)
		{
			Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
			Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
			Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
			Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

			GuiSystem::getInstance().getContext().drawFrame();

			Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
			Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
			Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::ScissorTest);
			Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::Blending);
		}
	}

	void DisplaySystemImpl::renderScene(const GDE::Timing& dt)
	{
		for (auto& display_component : _displayComponents)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				display_component->preRender();
			}
		}

		auto c = Scene::findEntitiesWithTag("camera");
		if(c)
		{
			auto camera = Scene::findEntitiesWithTag("camera")->back()->getComponent<CameraComponent>()->getCamera();
			camera->draw(_drawable);
			_shader->setProjectionMatrix(camera->projectionMatrix());

			for (auto& display_component : _displayComponents)
			{
				if (display_component->enabled() && display_component->owner().active())
				{
					display_component->display(*_shader.get(), dt);
				}
			}

			Magnum::GL::Renderer::setDepthFunction(Magnum::GL::Renderer::DepthFunction::LessOrEqual);
			//PhysicsSystem::getInstance().getDebugDraw()->setTransformationProjectionMatrix(
			//	camera->projectionMatrix() * camera->cameraMatrix());
			//PhysicsSystem::getInstance().getWorld()->debugDrawWorld();
			Magnum::GL::Renderer::setDepthFunction(Magnum::GL::Renderer::DepthFunction::Less);
		}
	}

	void DisplaySystemImpl::registerComponent(DisplayComponent* display_component)
	{
		_displayComponents.insert(display_component);
	}
	void DisplaySystemImpl::removeComponent(DisplayComponent* display_component)
	{
		_displayComponents.erase(display_component);
	}
}