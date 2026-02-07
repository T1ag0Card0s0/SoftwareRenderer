cmake_minimum_required(VERSION 3.16)
project(software_renderer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC
    src/main.c
    src/renderer.c
)

if (WIN32)
    list(APPEND SRC src/platform/win32.c)
    add_executable(renderer WIN32 ${SRC})
    target_link_libraries(renderer gdi32)

elseif(UNIX)
    find_package(X11 REQUIRED)
    list(APPEND SRC src/platform/x11.c)
    add_executable(renderer ${SRC})
    target_include_directories(renderer PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(renderer ${X11_LIBRARIES})
endif()

target_include_directories(renderer PRIVATE include)

