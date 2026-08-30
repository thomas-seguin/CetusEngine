#pragma once

#include "Renderer/FrameBuffer.h"

namespace Cetus {
    class OpenGLFramebuffer : public FrameBuffer {
    public:
        OpenGLFramebuffer(const FramebufferSpec& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        void Bind() override;
        void UnBind() override;

        uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

        const FramebufferSpec & GetSpec() const override { return m_Spec; }

    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpec m_Spec;
    };
}