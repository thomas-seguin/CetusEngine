#include "ctpch.h"
#include "Application.h"
#include "Events.h"

#include "Renderer/Renderer.h"


namespace Cetus {
    static Application* s_Application = nullptr;



    Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
        s_Application = this;
        Log::Init();
        CT_CORE_INFO("Application '{}' initialized", m_Specification.Name)

        m_Window = Window::Create(WindowProps(m_Specification.Name));
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        m_ImGuiLayer = std::make_unique<ImGuiLayer>();
        m_ImGuiLayer->Init();
    }

    Application::~Application() {
        m_ImGuiLayer->Shutdown();
        Log::GetCoreLogger()->info("Application shutting down");
        s_Application = nullptr;
    }

    void Application::Run() {
        m_Running = true;

        while (m_Running) {
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
            RenderCommand::Clear();

            Renderer::BeginScene();

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate(GetTime());

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();

            Renderer::EndScene();

            m_ImGuiLayer->Begin();
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::Stop() {
        m_Running = false;
    }

    void Application::OnEvent(Event& e) {

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