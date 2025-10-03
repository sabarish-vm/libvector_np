cmake_minimum_required(VERSION 3.5)
project(MyProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Conditionally choose files based on -DSELECTED_PROGRAM argument
if(MODE STREQUAL "DEBUG_VECTOR")
    set(SOURCES vector.cpp)
    set(EXEC "vec_debug.out")
    add_executable(${EXEC} ${SOURCES})
    target_include_directories(${EXEC} PRIVATE "./include")
    target_compile_definitions(${EXEC} PRIVATE -DDEBUG)
    target_compile_options(${EXEC} PRIVATE -Wall -Wextra -O2 -std=c++17)
else()
    message(FATAL_ERROR "Invalid program selected. Choose program_1,
    program_2 or program_3")
endif()
