#pragma once

#include "Layer.h"

#include <string>
#include <memory>
#include <vector>

namespace Cetus {

    struct ApplicationSpecification {
        std::string Name = "Application";
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification = ApplicationSpecification());
        ~Application();

        void Run();
        void Stop();

        template<typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer(){
            m_LayerStack.push_back(std::make_unique<TLayer>());
        }

        static Application& Get();
        static float GetTime();
    private:
        ApplicationSpecification m_Specification;
        bool m_Running = false;

        std::vector<std::unique_ptr<Layer>> m_LayerStack;
    };

}