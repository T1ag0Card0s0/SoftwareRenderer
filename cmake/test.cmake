include_guard(GLOBAL)

function(software_renderer_test)
  set(oneValueArgs TARGET)  
  set(multiValueArgs
        SOURCES
        INCLUDE_DIRS
        DEFINITIONS
        LIBS
    )

    cmake_parse_arguments(TEST
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT TEST_TARGET)
        message(FATAL_ERROR "software_renderer_test: TARGET is mandatory")
    endif()

    if(NOT TEST_SOURCES)
        message(FATAL_ERROR "software_renderer_test: SOURCES is mandatory")
    endif()


    if (WIN32)
        add_executable(${TEST_TARGET} WIN32 ${TEST_SOURCES})
    elseif (UNIX AND NOT APPLE)
        add_executable(${TEST_TARGET} ${TEST_SOURCES})
    else()
        add_executable(${TEST_TARGET} ${TEST_SOURCES})
    endif()

    if(TEST_INCLUDE_DIRS)
        target_include_directories(${TEST_TARGET} PRIVATE ${TEST_INCLUDE_DIRS})
    endif()

    if(TEST_DEFINITIONS)
        target_compile_definitions(${TEST_TARGET} PRIVATE ${TEST_DEFINITIONS})
    endif()

    if(TEST_LIBS)
        target_link_libraries(${TEST_TARGET} PRIVATE ${TEST_LIBS})
    endif()

    target_compile_options(${TEST_TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests/framework)
    target_link_libraries(${TEST_TARGET} sr_tests_framework)
   
    software_renderer_format_target(TARGET ${TEST_TARGET})
    
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endfunction()

