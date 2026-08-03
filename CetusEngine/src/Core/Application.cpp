#include "Application.h"
#include "Log.h"
#include "Events.h"
#include <cassert>

namespace Cetus {
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
        s_Application = this;
        Log::Init();
        CT_CORE_INFO("Application '{}' initialized", m_Specification.Name)

        m_Window = Window::Create(WindowProps(m_Specification.Name));
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
    }

    Application::~Application() {
        Log::GetCoreLogger()->info("Application shutting down");
        s_Application = nullptr;
    }

    void Application::Run() {
        m_Running = true;

        while (m_Running) {
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate(GetTime());

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();

            m_Window->OnUpdate();
        }
    }

    void Application::Stop() {
        m_Running = false;
    }

    void Application::OnEvent(Event& e) {
        CT_CORE_TRACE("{0}", e.ToString())

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) {
            Stop();
            return true;
        });

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    Application& Application::Get() {
        assert(s_Application);
        return *s_Application;
    }

    float Application::GetTime() {
        return 0.0f;
    }

}