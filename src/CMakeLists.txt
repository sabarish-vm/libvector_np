include_directories(tests PRIVATE ../include)
add_library(vector_lib STATIC)
target_sources(vector_lib PRIVATE vector.cpp)
