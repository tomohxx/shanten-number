cmake_minimum_required(VERSION 3.5)

project(shanten-number)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")
set(CMAKE_INSTALL_DATADIR share/data)

find_package(TBB REQUIRED)

option(ENABLE_NYANTEN "ENABLE_NYANTEN" off)
option(FIX_RANDOM_SEED "FIX_RANDOM_SEED" off)

add_library(calsht src/calsht.cpp)
if(ENABLE_NYANTEN)
  target_compile_definitions(calsht PUBLIC ENABLE_NYANTEN=true)
endif()
set_target_properties(calsht PROPERTIES PUBLIC_HEADER src/calsht.hpp)
install(TARGETS calsht EXPORT Calsht)
install(FILES
  ${PROJECT_BINARY_DIR}/index_s.bin
  ${PROJECT_BINARY_DIR}/index_h.bin
TYPE DATA)
install(EXPORT Calsht FILE CalshtConfig.cmake DESTINATION share/cmake)

add_executable(mkind src/mkind.cpp)
if(ENABLE_NYANTEN)
  target_compile_definitions(mkind PUBLIC ENABLE_NYANTEN=true)
endif()
target_link_libraries(mkind TBB::tbb)

add_executable(sample src/sample.cpp)
target_compile_definitions(sample PUBLIC INDEX_FILE_PATH="${PROJECT_BINARY_DIR}")
if(FIX_RANDOM_SEED)
  target_compile_definitions(sample PUBLIC FIX_RANDOM_SEED=true)
endif()
target_link_libraries(sample calsht)
