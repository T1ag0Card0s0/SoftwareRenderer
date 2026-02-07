include_guard(GLOBAL)

find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-18 clang-format-17 clang-format-16 clang-format-15
    DOC "Path to clang-format executable"
)

if(CLANG_FORMAT_EXECUTABLE)
    execute_process(
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --version
        OUTPUT_VARIABLE CLANG_FORMAT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXECUTABLE} (${CLANG_FORMAT_VERSION})")
else()
    message(STATUS "clang-format not found. Format targets will not be available.")
endif()

function(software_renderer_format_target)
    if(NOT CLANG_FORMAT_EXECUTABLE)
        return()
    endif()

    set(ARGS_FLAG)
    set(ARGS_VALUE
        TARGET
        NAME
    )
    set(ARGS_MULTI_VALUE
        SOURCES
    )
    cmake_parse_arguments(FMT "${ARGS_FLAG}" "${ARGS_VALUE}" "${ARGS_MULTI_VALUE}" ${ARGN})

    if(FMT_TARGET)
        get_target_property(TARGET_SOURCES ${FMT_TARGET} SOURCES)
        set(FORMAT_SOURCES ${TARGET_SOURCES})
        if(NOT FMT_NAME)
            set(FMT_NAME ${FMT_TARGET})
        endif()
    elseif(FMT_SOURCES)
        set(FORMAT_SOURCES ${FMT_SOURCES})
        if(NOT FMT_NAME)
            set(FMT_NAME "custom")
        endif()
    else()
        message(FATAL_ERROR "Either TARGET or SOURCES must be specified")
    endif()

    set(FORMATTABLE_SOURCES)
    foreach(SOURCE ${FORMAT_SOURCES})
        get_filename_component(EXT ${SOURCE} LAST_EXT)
        if(EXT MATCHES "\\.(c|cc|cpp|cxx|h|hh|hpp|hxx)$")
            list(APPEND FORMATTABLE_SOURCES ${SOURCE})
        endif()
    endforeach()

    if(NOT FORMATTABLE_SOURCES)
        message(STATUS "No formattable sources found for ${FMT_NAME}")
        return()
    endif()

    add_custom_target(format-${FMT_NAME}
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=file ${FORMATTABLE_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting ${FMT_NAME} with clang-format"
        VERBATIM
    )

    if(TARGET format)
        add_dependencies(format format-${FMT_NAME})
    endif()
endfunction()

function(software_renderer_format_project)
    if(NOT CLANG_FORMAT_EXECUTABLE)
      message(WARNING "There is no clang-format installed")
        return()
    endif()

    if(NOT TARGET format)
        add_custom_target(format
            COMMENT "Formatting all targets with clang-format"
        )
    endif()
endfunction()
