include_guard(GLOBAL)

function(software_renderer_module)
    set(oneValueArgs TARGET C_STANDARD)
    set(multiValueArgs
        SOURCES
        INCLUDE_DIRS
        PUBLIC_INCLUDE_DIRS
        DEFINITIONS
        PUBLIC_DEFINITIONS
        LIBS
        PUBLIC_LIBS
    )

    cmake_parse_arguments(MODULE
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT MODULE_TARGET)
        message(FATAL_ERROR "software_renderer_module: TARGET is mandatory")
    endif()

    if(NOT MODULE_SOURCES)
        message(FATAL_ERROR "software_renderer_module: SOURCES is mandatory")
    endif()

    add_library(${MODULE_TARGET} ${MODULE_SOURCES})

    if(MODULE_INCLUDE_DIRS)
        target_include_directories(${MODULE_TARGET} PRIVATE ${MODULE_INCLUDE_DIRS})
    endif()
    if(MODULE_PUBLIC_INCLUDE_DIRS)
        target_include_directories(${MODULE_TARGET} PUBLIC ${MODULE_PUBLIC_INCLUDE_DIRS})
    endif()

    if(MODULE_DEFINITIONS)
        target_compile_definitions(${MODULE_TARGET} PRIVATE ${MODULE_DEFINITIONS})
    endif()
    if(MODULE_PUBLIC_DEFINITIONS)
        target_compile_definitions(${MODULE_TARGET} PUBLIC ${MODULE_PUBLIC_DEFINITIONS})
    endif()

    if(MODULE_LIBS)
        target_link_libraries(${MODULE_TARGET} PRIVATE ${MODULE_LIBS})
    endif()
    if(MODULE_PUBLIC_LIBS)
        target_link_libraries(${MODULE_TARGET} PUBLIC ${MODULE_PUBLIC_LIBS})
    endif()

    target_compile_options(${MODULE_TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror)

    software_renderer_format_target(TARGET ${MODULE_TARGET})
endfunction()

function(software_renderer_executable)
    set(options ENABLE_WARNINGS WARNINGS_AS_ERRORS WIN32_APP)
    set(oneValueArgs TARGET C_STANDARD)
    set(multiValueArgs
        SOURCES
        INCLUDE_DIRS
        DEFINITIONS
        LIBS
    )

    cmake_parse_arguments(EXE
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT EXE_TARGET)
        message(FATAL_ERROR "software_renderer_executable: TARGET is mandatory")
    endif()

    if(NOT EXE_SOURCES)
        message(FATAL_ERROR "software_renderer_executable: SOURCES is mandatory")
    endif()

    if (WIN32)
        add_executable(${EXE_TARGET} WIN32 ${EXE_SOURCES})
    elseif (UNIX AND NOT APPLE)
        add_executable(${EXE_TARGET} ${EXE_SOURCES})
    endif()

    if(EXE_INCLUDE_DIRS)
        target_include_directories(${EXE_TARGET} PRIVATE ${EXE_INCLUDE_DIRS})
    endif()

    if(EXE_DEFINITIONS)
        target_compile_definitions(${EXE_TARGET} PRIVATE ${EXE_DEFINITIONS})
    endif()

    if(EXE_LIBS)
        target_link_libraries(${EXE_TARGET} PRIVATE ${EXE_LIBS})
    endif()

    target_compile_options(${EXE_TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror)

    software_renderer_format_target(TARGET ${EXE_TARGET})
endfunction()

