software_renderer_executable(
    TARGET renderer
    SOURCES
        main.c
        ui_menu.c
    LIBS
       renderer_core 
    INCLUDE_DIRS
      ${CMAKE_SOURCE_DIR}/resources
)
