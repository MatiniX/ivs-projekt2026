cmake_minimum_required(VERSION 3.16)

project(calculator_math VERSION 0.1 LANGUAGES CXX)

include(CTest)

add_library(calculator_math STATIC
    src/math.cpp
)

add_library(calculator::math ALIAS calculator_math)

target_include_directories(calculator_math
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(calculator_math PUBLIC cxx_std_17)

if(BUILD_TESTING)
    add_executable(calculator_math_tests
        tests/math_tests.cpp
    )

    target_link_libraries(calculator_math_tests PRIVATE calculator_math)
    target_compile_features(calculator_math_tests PRIVATE cxx_std_17)

    add_test(NAME calculator_math_tests COMMAND calculator_math_tests)
endif()