#include "ctpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Cetus {
    Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			CT_CORE_ERROR("{0}", infoLog.data());
			CT_CORE_ASSERT(false, "Vertex shader compilation error!");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			CT_CORE_ERROR("{0}", infoLog.data());
			CT_CORE_ASSERT(false, "Fragment shader compilation error!");
			return;
		}

		m_RenderID = glCreateProgram();

		glAttachShader(m_RenderID, vertexShader);
		glAttachShader(m_RenderID, fragmentShader);

		glLinkProgram(m_RenderID);

		GLint isLinked = 0;
		glGetProgramiv(m_RenderID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RenderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RenderID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RenderID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CT_CORE_ERROR("{0}", infoLog.data());
			CT_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		glDetachShader(m_RenderID, vertexShader);
		glDetachShader(m_RenderID, fragmentShader);
    }

    Shader::~Shader() {
    	glDeleteProgram(m_RenderID);
    }

    void Shader::Bind() const {
    	glUseProgram(m_RenderID);
    }

    void Shader::Unbind() const {
    	glUseProgram(0);
    }
}
