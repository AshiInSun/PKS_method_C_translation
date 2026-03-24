cmake_minimum_required(VERSION 3.29)
project(PKS_method_C_translation C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(src)

add_executable(PKS_method_C_translation
        src/main.c
        src/graph.c)
