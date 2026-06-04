#pragma once

#include "Core/Layer.h"

class AppLayer : public Cetus::Layer {
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};