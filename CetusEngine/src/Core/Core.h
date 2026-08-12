#pragma once

#ifdef CT_DEBUG
    #define CT_ENABLE_ASSERTS
#endif

#ifdef CT_ENABLE_ASSERTS
    #define CT_ASSERT(x, ...) { if(!(x)) { CT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define CT_CORE_ASSERT(x, ...) { if(!(x)) { CT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define CT_ASSERT(x, ...)
    #define CT_CORE_ASSERT(x, ...)
#endif
