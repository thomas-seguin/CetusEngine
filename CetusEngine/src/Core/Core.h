#pragma once

#include "Core/Log.h"

#ifdef CT_DEBUG
    #define CT_ENABLE_ASSERTS
#endif

#if defined(_MSC_VER)
    #define CT_DEBUGBREAK() __debugbreak()
#elif defined(__MINGW32__) || defined(__MINGW64__)
    #include <windows.h>
    #define CT_DEBUGBREAK() DebugBreak()
#elif defined(__clang__)
    #define CT_DEBUGBREAK() __builtin_debugtrap()
#elif defined(__GNUC__)
    #include <csignal>
    #define CT_DEBUGBREAK() raise(SIGTRAP)
#else
    #error "CT_DEBUGBREAK not implemented for this platform/compiler"
#endif

#ifdef CT_ENABLE_ASSERTS
    #define CT_ASSERT(x, ...) { if(!(x)) { CT_ERROR("Assertion Failed: {0}", __VA_ARGS__); CT_DEBUGBREAK(); } }
    #define CT_CORE_ASSERT(x, ...) { if(!(x)) { CT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); CT_DEBUGBREAK(); } }
#else
    #define CT_ASSERT(x, ...)
    #define CT_CORE_ASSERT(x, ...)
#endif
