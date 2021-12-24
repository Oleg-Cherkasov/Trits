cmake_minimum_required(VERSION 3.20)
project(Trits)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(googletest-main)

include_directories(googletest-main/googletest/include)
add_executable(Trits main.cpp trit.cpp trit.h test.cpp)
target_link_libraries(Trits gtest)