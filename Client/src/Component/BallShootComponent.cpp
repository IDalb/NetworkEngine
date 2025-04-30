#include "Component/BallShootComponent.h"
#include <Component/TransformComponent.h>
#include <Component/RigidbodyComponent.h>
#include <source_dir.h>
#include <Scene.h>
#include <Entity.h>
#include <System/InputSystem.h>
namespace Client
{
	void BallShootComponent::updateLogic(const GDE::Timing& timing)
	{
		if (GDE::InputSystem::getInstance().isMouseButtonPressed(GDE::Mouse::LEFT))
		{
			GDE::Description descr = GDE::Descr::load(std::string(SOURCE_DIR) + "/Game/data/template/ball.yaml");
			auto camera_transform = GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>();
			Magnum::Vector3 camera_pos = camera_transform->getTransform().transformationMatrix().translation();
			
			descr[0]["description"]["components"]["Transform"]["position"][0] = camera_pos.x();
			descr[0]["description"]["components"]["Transform"]["position"][1] = camera_pos.y();
			descr[0]["description"]["components"]["Transform"]["position"][2] = camera_pos.z();
			GDE::EntityRef ball = GDE::Scene::createEntity(descr[0], owner().shared_from_this());
			const Magnum::Vector3 direction(-camera_transform->getTransform().transformation().transformVector(Magnum::Vector3::zAxis()));

			ball->getComponent<GDE::RigidbodyComponent>()->getRigidBody().setLinearVelocity(btVector3{ direction * 25.f });
		}
	}
}