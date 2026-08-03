#include "GLFWWindow.h"
#include "Core/Log.h"
#include "Core/Events.h"

#include <glad/glad.h>

#include <cassert>

namespace Cetus {
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        CT_CORE_ERROR("GLFW Error ({0}): {1}", error, description)
    }

    std::unique_ptr<Window> Window::Create(const WindowProps &props) {
        return std::make_unique<GLFWWindow>(props);
    }

    GLFWWindow::GLFWWindow(const WindowProps &props) {
        Init(props);
    }

    GLFWWindow::~GLFWWindow() {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        CT_CORE_INFO("Creating window...");

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            assert(success && "Could not init GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(
                (int)props.Width,
                (int)props.Height,
                m_Data.Title.c_str(),
                nullptr,
                nullptr
            );
        assert(m_Window && "Failed to create window!");

        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        assert(status && "Failed to initialize GLAD");
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // --- Window events ---

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            if (data.EventCallback)
                data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event((uint32_t)width, (uint32_t)height);
            if (data.EventCallback)
                data.EventCallback(event);
        });

        // --- Key events ---

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!data.EventCallback)
                return;

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, false);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, true);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        // --- Mouse button events ---

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!data.EventCallback)
                return;

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        // --- Mouse move / scroll events ---

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)x, (float)y);
            if (data.EventCallback)
                data.EventCallback(event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            if (data.EventCallback)
                data.EventCallback(event);
        });
    }

    void GLFWWindow::Shutdown() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
    }

    void GLFWWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void GLFWWindow::SetVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

}