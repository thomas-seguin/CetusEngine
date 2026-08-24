#include "ctpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Cetus {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
