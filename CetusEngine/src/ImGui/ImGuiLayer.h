#pragma once
#include "Core/Layer.h"

namespace Cetus {
    class ImGuiLayer :public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void Init();
        void Shutdown();

        void Begin();
        void End();
    };
}