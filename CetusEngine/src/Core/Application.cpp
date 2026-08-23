#include "ctpch.h"
#include "Application.h"
#include "Events.h"

#include <glad/glad.h>

namespace Cetus {
    static Application* s_Application = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        }

        CT_CORE_ASSERT(false, "Unknown shader data type");
        return 0;
    }

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

        float vertices[3*7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"}
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)element.Offset
                );
            index++;
        }

        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));


        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec4 v_Color;
            void main()
            {
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0f);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec4 v_Color;

            void main()
            {
                color = vec4(0.8, 0.2, 0.3, 1.0);
                color = v_Color;
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