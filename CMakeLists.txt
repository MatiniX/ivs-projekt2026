cmake_minimum_required(VERSION 3.16)

project(ivs-projekt2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(src/math)
add_subdirectory(src/gui)