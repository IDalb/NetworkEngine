#include "Component/TransformComponent.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

namespace GDE
{
    void TransformComponent::setup(const ComponentDescription& init_value)
    {
        if (init_value.parameters.contains("position"))
        {
            _transform.translate({ init_value.parameters.at("position")[0].as<float>(),
                init_value.parameters.at("position")[1].as<float>(),
                init_value.parameters.at("position")[2].as<float>() }
            );
        }
        if (init_value.parameters.contains("rotation"))
        {
            _transform.rotate(Magnum::Deg(init_value.parameters.at("rotation")[0].as<float>())
            , { init_value.parameters.at("rotation")[1].as<float>(),
                 init_value.parameters.at("rotation")[2].as<float>(),
                 init_value.parameters.at("rotation")[3].as<float>() }
            );
        }
        if (init_value.parameters.contains("scale"))
        {
            _transform.scale({ init_value.parameters.at("scale")[0].as<float>(),
                 init_value.parameters.at("scale")[1].as<float>(),
                 init_value.parameters.at("scale")[2].as<float>() }
            );
        }
    }
}
