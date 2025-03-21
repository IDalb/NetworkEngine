#pragma once

#include <Component/Component.h>

namespace GDEEditor
{
	class SceneCleanerComponent : public GDE::Component
	{
	public:
		static constexpr auto type = "SceneCleaner";
		SceneCleanerComponent(GDE::Entity& parent) : Component(parent) {}
		~SceneCleanerComponent();
		void setup(const GDE::ComponentDescription& init_value) override {}
	};
}