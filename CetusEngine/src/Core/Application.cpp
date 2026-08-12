#include "ctpch.h"
#include "Application.h"
#include "Events.h"

#include <glad/glad.h>

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

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3*3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));


        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;


            void main()
            {
                gl_Position = vec4(a_Position, 1.0f);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;


            void main()
            {
                color = vec4(0.8, 0.2, 0.3, 1.0);
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

    }

    Application::~Application() {
        m_ImGuiLayer->Shutdown();
        Log::GetCoreLogger()->info("Application shutting down");
        s_Application = nullptr;
    }

    void Application::Run() {
        m_Running = true;

        while (m_Running) {

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate(GetTime());

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();

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