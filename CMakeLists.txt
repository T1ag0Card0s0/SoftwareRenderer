cmake_minimum_required(VERSION 3.20)
project(software_renderer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_SOURCES
    src/main.c
    src/renderer.c
)

if(WIN32)
    set(PLATFORM_SOURCES src/platform/win32.c)
    set(PLATFORM_LIBS gdi32)
    set(WIN32_EXECUTABLE WIN32)
elseif(UNIX)
    find_package(X11 REQUIRED)
    set(PLATFORM_SOURCES src/platform/x11.c)
    set(PLATFORM_LIBS ${X11_LIBRARIES})
    set(PLATFORM_INCLUDES ${X11_INCLUDE_DIR})
endif()

add_executable(renderer ${WIN32_EXECUTABLE}
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCES}
)

target_include_directories(renderer PRIVATE
    include
    ${PLATFORM_INCLUDES}
)

target_link_libraries(renderer PRIVATE
    ${PLATFORM_LIBS}
)

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE FORMAT_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/include/*.h
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${FORMAT_FILES}
        COMMENT "Formatting source files"
        VERBATIM
    )
endif()
