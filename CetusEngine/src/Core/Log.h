#pragma once

#include "spdlog/spdlog.h"

namespace Cetus {
    class Log {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

#define CT_CORE_TRACE(...) ::Cetus::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CT_CORE_INFO(...) ::Cetus::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CT_CORE_WARN(...) ::Cetus::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CT_CORE_ERROR(...) ::Cetus::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CT_CORE_CRITICAL(...) ::Cetus::Log::GetCoreLogger()->critical(__VA_ARGS__);

#define CT_TRACE(...) ::Cetus::Log::GetClientLogger()->trace(__VA_ARGS__);
#define CT_INFO(...) ::Cetus::Log::GetClientLogger()->info(__VA_ARGS__);
#define CT_WARN(...) ::Cetus::Log::GetClientLogger()->warn(__VA_ARGS__);
#define CT_ERROR(...) ::Cetus::Log::GetClientLogger()->error(__VA_ARGS__);
#define CT_CRITICAL(...) ::Cetus::Log::GetClientLogger()->critical(__VA_ARGS__);




}