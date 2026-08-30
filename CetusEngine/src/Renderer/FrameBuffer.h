#pragma once

namespace Cetus {
    struct FramebufferSpec {
        uint32_t Width, Height;
        uint32_t Samples = 1;
        bool SwapChainTarget = false;
    };

    class FrameBuffer {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual const FramebufferSpec& GetSpec() const = 0;

        static std::shared_ptr<FrameBuffer> Create(const FramebufferSpec& spec);
    };
}