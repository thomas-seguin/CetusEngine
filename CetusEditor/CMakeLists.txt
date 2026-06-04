add_executable(CetusEditor
    src/main.cpp
        src/AppLayer.cpp
        src/AppLayer.h
)

target_link_libraries(CetusEditor PRIVATE
        ArgusEngine
)

target_compile_features(CetusEditor PUBLIC cxx_std_20)