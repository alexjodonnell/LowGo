cmake_minimum_required(VERSION 3.9)
project(LowGo)

set(CMAKE_CXX_STANDARD 11)

add_executable(LowGo main.cpp shared/TGAImage.cpp shared/TGAImage.h shared/geometry.h shared/Model.cpp shared/Model.h shared/geometry.cpp GL/GL.cpp GL/GL.h shared/colors.h)