cmake_minimum_required(VERSION 3.16)

if(NOT TARGET calculator_math)
    message(FATAL_ERROR "Target calculator_math is missing. Configure from repository root using cmake -S . -B build.")
endif()

add_executable(stddev
    stddev.cpp
)

target_link_libraries(stddev PRIVATE calculator::math)
target_compile_features(stddev PRIVATE cxx_std_17)
