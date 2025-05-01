#include "Component/BallShootComponent.h"
#include <Component/TransformComponent.h>
#include <Component/RigidbodyComponent.h>
#include <source_dir.h>
#include <Scene.h>
#include <Entity.h>
#include <System/ServerInputSystem.h>
#include "Component/Player.h"
namespace Server
{
	void BallShootComponent::updateLogic(const GDE::Timing& timing)
	{
		if (GDE::ServerInputSystem::getInstance().isMouseButtonPressed(GDE::Mouse::Button::LEFT, _netId = owner().getComponent<Player>()->getNetId()))
		{
			GDE::Description descr = GDE::Descr::load(std::string(SOURCE_DIR) + "/Server/data/template/Ball.yaml");
			auto camera_transform = owner().getComponent<GDE::TransformComponent>();
			Magnum::Vector3 camera_pos = camera_transform->getTransform().transformationMatrix().translation();
			
			descr[0]["description"]["components"]["Transform"]["position"][0] = camera_pos.x();
			descr[0]["description"]["components"]["Transform"]["position"][1] = camera_pos.y();
			descr[0]["description"]["components"]["Transform"]["position"][2] = camera_pos.z();
			GDE::EntityRef ball = GDE::Scene::createEntity(descr[0], owner().getParent(), "Ball.yaml");
			const Magnum::Vector3 direction(-camera_transform->getTransform().transformation().transformVector(Magnum::Vector3::zAxis()));

			ball->getComponent<GDE::RigidbodyComponent>()->getRigidBody().setLinearVelocity(btVector3{ direction * 25.f });
		}
	}
	void BallShootComponent::resolve()
	{
	}
}