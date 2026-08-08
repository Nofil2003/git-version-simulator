cmake_minimum_required(VERSION 3.16)
project(minigit CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "src/*.cpp")

add_executable(minigit ${SOURCES})
target_include_directories(minigit PRIVATE src)