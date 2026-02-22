cmake_minimum_required(VERSION 3.20)
project(software_renderer C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -O2)

set(FRAMEBUFFER_WIDTH 640)
set(FRAMEBUFFER_HEIGHT 480)
set(MAX_FRAMES_PER_SECOND 30)
configure_file(${CMAKE_SOURCE_DIR}/include/renderer/config.h.in 
  ${CMAKE_SOURCE_DIR}/include/renderer/config.h
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SoftwareRenderer)

software_renderer_module(
    TARGET 
      sr_core
    SOURCES
      src/renderer.c
      src/framebuffer.c
      src/primitive.c
      src/algebra.c
      src/text.c
    PUBLIC_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/include
    LIBS
       m
)

add_subdirectory(platform)
add_subdirectory(demo)
add_subdirectory(tests)
