#include "ctpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include "Renderer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Cetus {
	std::shared_ptr<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     CT_CORE_ASSERT(false, "Render API none is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(filepath);
		}

		CT_CORE_ASSERT(false, "Unknown/Invalid Renderer API");
		return nullptr;
	}
}
