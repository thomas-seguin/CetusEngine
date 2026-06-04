#include "Application.h"

#include <cassert>

namespace Cetus {
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
        s_Application = this;
    }

    Application::~Application() {
        s_Application = nullptr;
    }

    void Application::Run() {
        m_Running = true;

        while (m_Running) {
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate(GetTime());

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();
        }
    }

    void Application::Stop() {
        m_Running = false;
    }

    Application& Application::Get() {
        assert(s_Application);
        return *s_Application;
    }

    float Application::GetTime() {
        return 0.0f;
    }

}