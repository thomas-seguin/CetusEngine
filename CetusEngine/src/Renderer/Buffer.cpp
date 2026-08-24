#include "ctpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Cetus {
    VertexBuffer * VertexBuffer::Create(float *vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:     CT_CORE_ASSERT(false, "Render API none is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
        }

        CT_CORE_ASSERT(false, "Unknown/Invalid Renderer API");
        return nullptr;
    }

    IndexBuffer * IndexBuffer::Create(uint32_t *indices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:     CT_CORE_ASSERT(false, "Render API none is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(indices, size);
        }

        CT_CORE_ASSERT(false, "Unknown/Invalid Renderer API");
        return nullptr;
    }
}
