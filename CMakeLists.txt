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

if(WIN32)
    software_renderer_module(
        TARGET sr_platform
        SOURCES
            src/platform/win32.c
        INCLUDE_DIRS
            include
        LIBS
            gdi32
    )
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    software_renderer_module(
        TARGET sr_platform
        SOURCES
            src/platform/x11.c
        INCLUDE_DIRS
            include
            ${X11_INCLUDE_DIR}
        LIBS
            ${X11_LIBRARIES}
    )
endif()

# Demo application
software_renderer_executable(
    TARGET renderer
    SOURCES
        src/main.c
    LIBS
        sr_core
        sr_platform
)
