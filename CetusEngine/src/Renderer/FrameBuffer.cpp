#include "ctpch.h"
#include "FrameBuffer.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Cetus {
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FramebufferSpec &spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    CT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
        }

        CT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
