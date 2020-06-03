cmake_minimum_required(VERSION 3.10)

project(renderer CXX)

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_17)

add_executable(tinyrenderer
    IShader.cpp
    Shader.cpp
    TGAColor.cpp
    geometry.cpp
    main.cpp
    model.cpp
    our_gl.cpp
    tgaimage.cpp
)

target_link_libraries(tinyrenderer
    PRIVATE
    ProjectOptions
    )

