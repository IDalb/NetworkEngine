#pragma once
#include "Component/RigidbodyComponent.h"
#include "Component/ColoredDrawableComponent.h"

class Cube : public GDE::RigidbodyComponent, public GDE::ColoredDrawableComponent
{
public:
    static constexpr auto _type = "Cube";

    Cube(GDE::Entity owner) : Component(owner) {}

    void setup(const GDE::ComponentDescription &init_value) override;
    void update(const GDE::ComponentDescription &init_value) override;
private:
    Color3 _color;
    Vector3 _size;
};
