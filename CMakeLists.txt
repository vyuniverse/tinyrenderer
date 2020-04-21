cmake_minimum_required(VERSION 3.10)

project(tinyrenderer CXX)

include(StandardProjectSettings.cmake)
include(CompilerWarnings.cmake)

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_17)

add_library(ProjectWarnings INTERFACE)
set_project_warnings(ProjectWarnings)

add_executable(tinyrenderer
    geometry.cpp
    main.cpp
    model.cpp
    our_gl.cpp
    tgaimage.cpp
)

target_link_libraries(tinyrenderer
    PRIVATE
    ProjectOptions
    ProjectWarnings
)

