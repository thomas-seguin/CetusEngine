#pragma once
#include "Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Cetus {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filepath);
        ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t m_RendererID;
        std::string m_Filepath;
    };
}
