find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

software_renderer_module(
    TARGET platform
    SOURCES linux_gl.c
    LIBS
        X11::X11
        OpenGL::GL
        GLEW::GLEW
)
