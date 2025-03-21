#include "Component/TransformComponent.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include "Scene.h"
#include "Entity.h"

namespace GDE
{
    TransformComponent::~TransformComponent()
    {
        if(_transform->parent() == &Scene::getMagnumScene())
            delete _transform;
    }

    void TransformComponent::setup(const ComponentDescription& init_value)
    {
        _transform = new Object3D();
        //_transform.setParent(&Scene::getMagnumScene());

        if (init_value.parameters.contains("position"))
        {
            _transform->translate({ init_value.parameters.at("position")[0].as<float>(),
                init_value.parameters.at("position")[1].as<float>(),
                init_value.parameters.at("position")[2].as<float>() }
            );
        }
        if (init_value.parameters.contains("rotation"))
        {
            _transform->rotate(Magnum::Deg(init_value.parameters.at("rotation")[0].as<float>())
            , { init_value.parameters.at("rotation")[1].as<float>(),
                 init_value.parameters.at("rotation")[2].as<float>(),
                 init_value.parameters.at("rotation")[3].as<float>() }
            );
        }
        if (init_value.parameters.contains("scale"))
        {
            _transform->scale({ init_value.parameters.at("scale")[0].as<float>(),
                 init_value.parameters.at("scale")[1].as<float>(),
                 init_value.parameters.at("scale")[2].as<float>() }
            );
        }
    }
    void TransformComponent::resolve()
    {
        if (auto transform = owner().getParent()->getComponent<TransformComponent>())
        {
            _transform->setParent(transform->_transform);
        }
        else
        {
            _transform->setParent(&Scene::getMagnumScene());
        }
    }
}
