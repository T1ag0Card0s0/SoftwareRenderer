software_renderer_executable(
    TARGET renderer
    SOURCES
        main.c
    LIBS
        sr_core
        sr_platform
)
