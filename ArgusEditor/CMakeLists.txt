add_executable(ArgusEditor
    src/main.cpp
        src/AppLayer.cpp
        src/AppLayer.h
)

target_link_libraries(ArgusEditor PRIVATE
        ArgusEngine
)

target_compile_features(ArgusEditor PUBLIC cxx_std_20)