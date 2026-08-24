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

        m_VertexArray.reset(VertexArray::Create());

        float vertices[3*7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
            };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        m_SquareVA.reset(VertexArray::Create());

        float squareVertices[3*4] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout( {
            {ShaderDataType::Float3, "a_Position"},
        });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        m_Shader = Shader::Create("assets/shaders/Triangle.glsl");
        m_SquareShader = Shader::Create("assets/shaders/Square.glsl");

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

            m_SquareShader->Bind();
            Renderer::Submit(m_SquareVA);

            m_Shader->Bind();
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

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