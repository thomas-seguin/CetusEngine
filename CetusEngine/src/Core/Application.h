#pragma once

#include "Layer.h"
#include "Window.h"

#include <string>
#include <memory>
#include <vector>

#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"

#include "Renderer/Shader.h"

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

        inline Window& GetWindow() { return *m_Window; }


        static Application& Get();
        static float GetTime();
    private:

        void OnEvent(Event& e);
        ApplicationSpecification m_Specification;
        bool m_Running = false;

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
        std::vector<std::unique_ptr<Layer>> m_LayerStack;

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

    };

}
