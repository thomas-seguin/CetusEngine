#pragma once

struct GLFWwindow;

namespace Cetus {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static void ConfigureWindowHints();
        static GraphicsContext* Create(GLFWwindow* window);
    };
}