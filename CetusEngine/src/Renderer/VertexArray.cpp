#include "ctpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Cetus {
    VertexArray* VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:     CT_CORE_ASSERT(false, "Render API none is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();
        }

        CT_CORE_ASSERT(false, "Unknown/Invalid Renderer API");
        return nullptr;
    }
}
