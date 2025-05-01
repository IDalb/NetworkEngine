#include "Component/Player.h"
#include "Server.h"
#include <Entity.h>
#include <Component/ColoredDrawableComponent.h>
#include <System/ServerInputSystem.h>
#include <System/ServerNetworkSystem.h>
#include <Component/TransformComponent.h>

void Server::Player::updateLogic(const GDE::Timing& timing)
{
    if(_playerNetId == UINT32_MAX)
    {
        if (GDE::ServerNetworkSystem::getInstance()._gameIdToNetId.contains(_playerId))
        {
            _playerNetId = GDE::ServerNetworkSystem::getInstance()._gameIdToNetId.at(_playerId);
        }
    }
    else
    {
        auto& input = GDE::ServerInputSystem::getInstance();
        _playerRotation = owner().getComponent<GDE::TransformComponent>()->getTransform().transformation();
        // Rotation
        if (!input.isMouseButtonHeld(GDE::Mouse::Button::RIGHT, _playerNetId))
        {
            const Magnum::Vector2 mouseMvt = input.getMouseVelocity(_playerNetId) * PLAYER_ROTATION_SPEED;
            _xAngle += static_cast<Magnum::Deg>(mouseMvt.x());
            _yAngle += static_cast<Magnum::Deg>(mouseMvt.y());

            _playerRotation[0] = _playerBaseRotation[0];
            _playerRotation[1] = _playerBaseRotation[1];
            _playerRotation[2] = _playerBaseRotation[2];

            _playerRotation = _playerRotation
                * Magnum::Matrix4::rotation(-_xAngle, Magnum::Vector3::zAxis())
                * Magnum::Matrix4::rotationX(-_yAngle);
            _playerRotation.lookAt(_playerRotation.translation(), _playerRotation.translation() - _playerRotation.backward(), Magnum::Vector3::zAxis());

        }

        // Translation
        const int xAxis = input.getAxis(GDE::Key::Key::A, GDE::Key::Key::D, _playerNetId);
        int yAxis = input.getAxis(GDE::Key::Key::S, GDE::Key::Key::W, _playerNetId);
        const int zAxis = input.getAxis(GDE::Key::Key::Q, GDE::Key::Key::E, _playerNetId);


        _playerRotation[3] += playerTranslationSpeed * Magnum::Vector4(0, 0, static_cast<float>(zAxis), 0);
        _playerRotation = _playerRotation * Magnum::Matrix4::translation(Magnum::Vector3(static_cast<float>(xAxis), 0, -1.5f * static_cast<float>(yAxis)) * playerTranslationSpeed);

        owner().getComponent<GDE::TransformComponent>()->getTransform().setTransformation(_playerRotation);
    }
}

void Server::Player::resolve()
{
	const Magnum::Vector3& color = owner().getComponent<GDE::ColoredDrawableComponent>()->getColor();
	for (uint32_t i = 0; i < PLAYERS_COLOR.size(); i++)
	{
		if (PLAYERS_COLOR[i] == color)
		{
			_playerId = i;
			break;
		}
	}
    _playerRotation = ComputeCameraView(*owner().getComponent<GDE::TransformComponent>());

    //_CameraView = Magnum::Matrix4::lookAt(Magnum::Vector3{ owner().getComponent<TransformComponent>()->getTransform().absoluteTransformation().translation() }, Magnum::Vector3{ 0.0f, 0.0f, 0.0f }, Magnum::Vector3{ 0.0f, 0.0f, 1.0f });
    owner().getComponent<GDE::TransformComponent>()->getTransform().setTransformation(_playerRotation);
}

Magnum::Matrix4 Server::Player::ComputeCameraView(GDE::TransformComponent& transform) {
    auto cameraView = Magnum::Matrix4();
    _xAngle = static_cast<Magnum::Deg>(transform._rotationVector.x());
    _yAngle = static_cast<Magnum::Deg>(transform._rotationVector.y());
    cameraView = cameraView
        * Magnum::Matrix4::translation(transform.getTransform().absoluteTransformation().translation())
        * Magnum::Matrix4::rotation(static_cast<Magnum::Deg>(-transform._rotationVector.x()), Magnum::Vector3::zAxis())
        * Magnum::Matrix4::rotationX(static_cast<Magnum::Deg>(90 + transform._rotationVector.y()));
    ;

    return cameraView;
}
