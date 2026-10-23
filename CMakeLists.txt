
cmake_minimum_required(VERSION 3.20)

project(DimensionGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Use the Raylib source already downloaded locally
add_subdirectory(raylib-test raylib-build)

add_executable(DimensionGame
    src/main.cpp
)

target_link_libraries(DimensionGame PRIVATE raylib)

if(MINGW)
    target_link_options(DimensionGame PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

set_target_properties(DimensionGame PROPERTIES
    OUTPUT_NAME "PingPong"
)
