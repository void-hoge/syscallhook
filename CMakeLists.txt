cmake_minimum_required(VERSION 3.10)

project(syscallhook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./include)

add_library(syscallhook SHARED
    src/syscallhook.cpp
    example/userhook.cpp
)

target_link_libraries(syscallhook
    PRIVATE dl
)

target_compile_options(syscallhook
    PRIVATE -Wall
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter
)
