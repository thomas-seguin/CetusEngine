#pragma once
#include "Event.h"

namespace Cetus {
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void OnEvent(Event& event) {}

        virtual void OnUpdate(float ts) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };
}
