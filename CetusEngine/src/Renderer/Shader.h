#pragma once
#include <string>

namespace Cetus {
    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static std::shared_ptr<Shader> Create(const std::string& filepath);
    };
}
