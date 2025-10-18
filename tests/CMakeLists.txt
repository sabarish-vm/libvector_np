add_executable(tests test_vector.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests PRIVATE doctest::doctest vector_lib)
target_include_directories(tests PRIVATE ../include)
