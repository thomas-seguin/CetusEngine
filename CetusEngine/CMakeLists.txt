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
)

target_include_directories(CetusEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CetusEngine PUBLIC spdlog::spdlog)
target_link_libraries(CetusEngine PUBLIC glfw)
target_link_libraries(CetusEngine PUBLIC glad)

target_compile_features(CetusEngine PUBLIC cxx_std_20)