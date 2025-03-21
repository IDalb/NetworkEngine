#include "Editor/System/EditorDisplaySystemImpl.h"
#include <Game.h>
namespace GDEEditor
{
	void EditorDisplaySystemImpl::iterate(const GDE::Timing& dt)
	{
		if(_enable)
		{
			Vector2i framebufferSize = GDE::Game::_app->framebufferSize();
			// Calculate viewport position from the bottom-left
			Vector2i viewportPos = Vector2i(framebufferSize.x() * 0.15, framebufferSize.y() * 0.3);

			// Calculate viewport size
			Vector2i viewportSize = Vector2i(framebufferSize.x() * 0.85, framebufferSize.y());

			// Set viewport for scene rendering
			GL::defaultFramebuffer.setViewport({ viewportPos, viewportSize });
			renderScene(dt);
			GL::defaultFramebuffer.setViewport({ {}, framebufferSize });
		}

		renderUi();
	}
}