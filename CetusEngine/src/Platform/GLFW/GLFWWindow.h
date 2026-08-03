#pragma once

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Cetus {
    class GLFWWindow : public Window {
    public:
        explicit GLFWWindow(const WindowProps& props);
        virtual ~GLFWWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override {return m_Data.Width;}
        uint32_t GetHeight() const override {return m_Data.Height;}

        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }

        void* GetNativeWindow() const override { return m_Window; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        GLFWwindow* m_Window = nullptr;
        GraphicsContext* m_Context;

        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync = true;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
