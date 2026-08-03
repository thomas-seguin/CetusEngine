#pragma once

#include "Core/Layer.h"
#include "Core/Events.h"

class AppLayer : public Cetus::Layer {
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnEvent(Cetus::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};