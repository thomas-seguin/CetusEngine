#include "AppLayer.h"
#include "Core/Log.h"

#include "Renderer/Renderer.h"

#include "imgui.h"

AppLayer::AppLayer() {
    Cetus::Log::GetClientLogger()->info("AppLayer created");

    m_VertexArray.reset(Cetus::VertexArray::Create());

    float vertices[3*7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };

    m_VertexBuffer.reset(Cetus::VertexBuffer::Create(vertices, sizeof(vertices)));
    Cetus::BufferLayout layout = {
        {Cetus::ShaderDataType::Float3, "a_Position"},
        {Cetus::ShaderDataType::Float4, "a_Color"}
        };
    m_VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    uint32_t indices[3] = { 0, 1, 2 };
    m_IndexBuffer.reset(Cetus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_SquareVA.reset(Cetus::VertexArray::Create());

    float squareVertices[3*4] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };

    std::shared_ptr<Cetus::VertexBuffer> squareVB;
    squareVB.reset(Cetus::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVB->SetLayout( {
        {Cetus::ShaderDataType::Float3, "a_Position"},
    });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<Cetus::IndexBuffer> squareIB;
    squareIB.reset(Cetus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);

    m_Shader = Cetus::Shader::Create("assets/shaders/Triangle.glsl");
    m_SquareShader = Cetus::Shader::Create("assets/shaders/Square.glsl");
}

AppLayer::~AppLayer() {
    Cetus::Log::GetClientLogger()->info("AppLayer destroyed");
}

void AppLayer::OnEvent(Cetus::Event &event) {
    Cetus::EventDispatcher dispatcher(event);
}


void AppLayer::OnUpdate(float ts) {

}

void AppLayer::OnRender() {
    m_SquareShader->Bind();
    Cetus::Renderer::Submit(m_SquareVA);

    m_Shader->Bind();
    Cetus::Renderer::Submit(m_VertexArray);
}

void AppLayer::OnImGuiRender() {
    ImGui::ShowDemoWindow();
}