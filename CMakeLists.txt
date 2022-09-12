cmake_minimum_required(VERSION 3.23)
project(Compiler_Project)

set(CMAKE_CXX_STANDARD 23)

add_executable(Compiler_Project main.cpp "Source Files/FileFunctions.cpp" "Header Files/FileFunctions.h" Classes/Compiler.cpp "Header Files/Compiler.h")
