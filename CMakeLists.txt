cmake_minimum_required(VERSION 3.20)
project(software_renderer C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SoftwareRenderer)

software_renderer_module(
    TARGET sr_core
    SOURCES
        src/renderer.c
    PUBLIC_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(platform)
add_subdirectory(demo)
