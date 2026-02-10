find_package(X11 REQUIRED)
software_renderer_module(
    TARGET sr_platform
    SOURCES
        xlib.c
    INCLUDE_DIRS
        ${X11_INCLUDE_DIR}
    LIBS
        ${X11_LIBRARIES}
)
