add_library(CetusEngine STATIC
        src/Core/Layer.h
        src/Core/Event.h
        src/Core/Application.h
        src/Core/Application.cpp
)

target_include_directories(CetusEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(CetusEngine PUBLIC cxx_std_20)