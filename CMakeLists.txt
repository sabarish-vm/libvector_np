cmake_minimum_required(VERSION 3.5)
project(MyProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(ext/Catch2)
add_subdirectory(src)
add_subdirectory(tests)

# Custom target to clean everything except Catch2
add_custom_target(clean_except_catch2
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning build directory while preserving Catch2 compilation artifacts"
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/src
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/tests
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Testing
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/DartConfiguration.tcl
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E echo "Clean completed! Catch2 object files preserved."
)
