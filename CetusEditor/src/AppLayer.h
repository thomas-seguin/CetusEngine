#pragma once

#include "Core/Layer.h"
#include "Core/Events.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <memory>

class AppLayer : public Cetus::Layer {
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnEvent(Cetus::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
    void OnImGuiRender() override;

private:
    std::shared_ptr<Cetus::Shader> m_Shader;
    std::shared_ptr<Cetus::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Cetus::IndexBuffer> m_IndexBuffer;
    std::shared_ptr<Cetus::VertexArray> m_VertexArray;

    std::shared_ptr<Cetus::Shader> m_SquareShader;
    std::shared_ptr<Cetus::VertexArray> m_SquareVA;
};