cmake_minimum_required(VERSION 3.10)

project(tinyrenderer CXX)

add_executable(tinyrenderer
    geometry.cpp
    main.cpp
    model.cpp
    our_gl.cpp
    tgaimage.cpp
)

