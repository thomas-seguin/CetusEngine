#include "AppLayer.h"
#include "Core/Log.h"

#include "Renderer/Renderer.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AppLayer::AppLayer() {
    Cetus::Log::GetClientLogger()->info("AppLayer created");

    Cetus::FramebufferSpec fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Cetus::FrameBuffer::Create(fbSpec);

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
    // CAMERA SETUP
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

    m_Framebuffer->Bind();

    m_SquareShader->Bind();
    m_SquareShader->SetMat4("u_Model", model);
    m_SquareShader->SetMat4("u_View", view);
    m_SquareShader->SetMat4("u_Projection", projection);
    Cetus::Renderer::Submit(m_SquareVA);

    // m_Shader->Bind();
    // Cetus::Renderer::Submit(m_VertexArray);

    m_Framebuffer->UnBind();
}

void AppLayer::OnImGuiRender() {
    ImGui::DockSpaceOverViewport();

    ImGui::Begin("Viewport");
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((ImTextureID)(intptr_t)textureID,
                 ImGui::GetContentRegionAvail(),
                 ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();

    ImGui::Begin("Scene Hierarchy");
    ImGui::Text("Scene / entity list will go here.");
    ImGui::End();

    ImGui::Begin("Properties");
    ImGui::Text("Selected entity's properties will go here.");
    ImGui::End();
}