#pragma once
#include <Component/LogicComponent.h>

namespace GDE
{
	class TransformComponent;
}
namespace Server
{
	class Player : public GDE::LogicComponent
	{
	public:
		static constexpr auto type = "Player";


		Player(GDE::Entity& parent) : GDE::Component(parent) {}
		void setup(const GDE::ComponentDescription& init_value) override {}
		void updateLogic(const GDE::Timing& timing);
		void resolve() override;
		Magnum::Matrix4 ComputeCameraView(GDE::TransformComponent& transform);

		uint32_t getNetId() const { return _playerNetId; }
	private:
		uint32_t _playerId = 0;
		uint32_t _playerNetId = UINT32_MAX;

		float playerTranslationSpeed = .25f;
		static constexpr float PLAYER_ROTATION_SPEED = .115f;

		Magnum::Deg _xAngle = Magnum::Deg(0.0f);
		Magnum::Deg _yAngle = Magnum::Deg(0.0f);

		Magnum::Matrix4 _playerRotation;
		Magnum::Matrix4 _playerBaseRotation;
	};
}