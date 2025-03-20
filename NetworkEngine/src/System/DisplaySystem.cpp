#include "System/DisplaySystem.h"
#include "Component/DisplayComponent.h"

#include "Entity.h"
#include "GlobalConstants.h"
#include "System/GuiSystem.h"

#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Integration.h>
#include <Magnum/Math/Color.h>

using namespace Magnum::Math::Literals;

namespace GDE
{
	DisplaySystem& DisplaySystem::getInstance()
	{
		static DisplaySystem display_system;
		return display_system;
	}


	void DisplaySystem::setup()
	{
		_shader = Magnum::Shaders::PhongGL{ Magnum::Shaders::PhongGL::Configuration{}
			   .setFlags(Magnum::Shaders::PhongGL::Flag::VertexColor |
						 Magnum::Shaders::PhongGL::Flag::InstancedTransformation) };
		_shader.setAmbientColor(0x111111_rgbf)
			.setSpecularColor(0x330000_rgbf)
			.setLightPositions({ {10.0f, 15.0f, 5.0f, 0.0f} });
	}

	void DisplaySystem::iterate(const Timing& dt)
	{
		for (auto& display_component : _displayComponents)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				display_component->display(_shader, dt);
			}
		}
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

	void DisplaySystem::registerComponent(DisplayComponent* display_component)
	{
		_displayComponents.insert(display_component);
	}
	void DisplaySystem::removeComponent(DisplayComponent* display_component)
	{
		_displayComponents.erase(display_component);
	}
}