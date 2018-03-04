cmake_minimum_required(VERSION 3.9)
project(LowGo)

set(CMAKE_CXX_STANDARD 11)

add_executable(LowGo main.cpp tga/TGAImage.cpp tga/TGAImage.h tga/DWGTool.cpp tga/DWGTool.h geometry/geometry.h model/model.cpp model/model.h geometry/Triangle.cpp geometry/Triangle.h geometry/geometry.cpp)