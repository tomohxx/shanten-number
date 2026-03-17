cmake_minimum_required(VERSION 3.12)

project(shanten-number LANGUAGES CXX)

option(ENABLE_NYANTEN "ENABLE_NYANTEN" ON)
option(FIX_RANDOM_SEED "FIX_RANDOM_SEED" OFF)

find_package(TBB QUIET)
message(STATUS "TBB found: ${TBB_FOUND}")

add_library(compile_settings INTERFACE)
target_compile_features(compile_settings INTERFACE cxx_std_20)
target_compile_options(compile_settings INTERFACE
  $<$<CONFIG:Debug>:-O0 -Wall -Wextra -g>
  $<$<CONFIG:Release>:-O3 -Wall -Wextra>)

add_library(calsht src/calsht.cpp)
target_link_libraries(calsht compile_settings)
target_include_directories(calsht PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(calsht INTERFACE
  $<BUILD_INTERFACE:INDEX_FILE_PATH="${PROJECT_BINARY_DIR}">
  $<INSTALL_INTERFACE:INDEX_FILE_PATH="${CMAKE_INSTALL_PREFIX}/share/data/mahjong">
  PUBLIC $<$<BOOL:${ENABLE_NYANTEN}>:ENABLE_NYANTEN>)
install(TARGETS calsht compile_settings EXPORT Calsht)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/mahjong DESTINATION include)
install(FILES
  ${PROJECT_BINARY_DIR}/index_s.bin
  ${PROJECT_BINARY_DIR}/index_h.bin
  DESTINATION share/data/mahjong)
install(EXPORT Calsht
  FILE CalshtConfig.cmake
  NAMESPACE Mahjong::
  DESTINATION share/cmake/mahjong)

add_executable(mkind src/mkind.cpp)
target_compile_definitions(mkind PRIVATE
  $<$<BOOL:${ENABLE_NYANTEN}>:ENABLE_NYANTEN>
  $<$<BOOL:${TBB_FOUND}>:ENABLE_PARALLEL>)
target_link_libraries(mkind compile_settings $<$<BOOL:${TBB_FOUND}>:TBB::tbb>)

add_executable(example example/example.cpp)
target_compile_definitions(example PRIVATE $<$<BOOL:${FIX_RANDOM_SEED}>:FIX_RANDOM_SEED>)
target_link_libraries(example calsht)
