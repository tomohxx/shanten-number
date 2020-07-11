cmake_minimum_required(VERSION 3.1)

project(shanten-number-calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -DCHECK_HAND")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

option(THREE_PLAYER "Three player mode" off)

if(THREE_PLAYER)
  add_definitions(-DTHREE_PLAYER)
endif()

add_executable(mkind1 mkind1.cpp judwin.cpp)
add_executable(sample sample.cpp calsht.cpp)