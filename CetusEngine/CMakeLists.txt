set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

# SPD - Logger
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY  https://github.com/gabime/spdlog
        GIT_TAG         v1.17.0
)
FetchContent_MakeAvailable(spdlog)

#GLFW - Universal Window Manager
FetchContent_Declare(
        glfw
        GIT_REPOSITORY  https://github.com/glfw/glfw
        GIT_TAG         3.4
)
FetchContent_MakeAvailable(glfw)

# GLAD - OpenGL Loader
FetchContent_Declare(
        glad
        GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
        GIT_TAG         v0.1.36
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
FetchContent_MakeAvailable(glad)

# ImGui — docking branch, no CMakeLists of its own, so build it by hand
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

add_library(ImGui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_compile_definitions(ImGui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(ImGui PUBLIC glfw glad)

add_library(CetusEngine STATIC
        src/Core/Layer.h
        src/Core/Event.h
        src/Core/Application.h
        src/Core/Application.cpp
        src/Core/Log.h
        src/Core/Log.cpp
        src/Core/Window.h
        src/Platform/GLFW/GLFWWindow.h
        src/Platform/GLFW/GLFWWindow.cpp
        src/Core/Events.h
        src/ImGui/ImGuiLayer.h
        src/ImGui/ImGuiLayer.cpp
)

target_include_directories(CetusEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CetusEngine PUBLIC spdlog::spdlog)
target_link_libraries(CetusEngine PUBLIC glfw)
target_link_libraries(CetusEngine PUBLIC glad)
target_link_libraries(CetusEngine PUBLIC ImGui)

target_compile_features(CetusEngine PUBLIC cxx_std_20)