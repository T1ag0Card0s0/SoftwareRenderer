find_package(X11 REQUIRED)
software_renderer_module(
    TARGET platform
    SOURCES
        xlib.c
    INCLUDE_DIRS
        ${X11_INCLUDE_DIR}
    PUBLIC_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
    LIBS
        ${X11_LIBRARIES}        
        pthread
)
