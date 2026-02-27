cmake_minimum_required(VERSION 3.20)
project(software_renderer C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

set(PIXEL_BIT_SIZE 32)
configure_file(${CMAKE_SOURCE_DIR}/include/renderer/config.h.in 
  ${CMAKE_SOURCE_DIR}/include/renderer/config.h
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SoftwareRenderer)

software_renderer_module(
    TARGET 
      sr_core
    SOURCES
        src/framebuffer.c 
        src/renderer.c
        src/arena.c
        src/primitive.c
        src/algebra.c
        src/camera.c
        src/text.c
    PUBLIC_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/include
    LIBS
       m
)

add_subdirectory(platform)
add_subdirectory(demo)
add_subdirectory(tests)
