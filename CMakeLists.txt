cmake_minimum_required(VERSION 3.18.4)

include(cmake/conf.cmake)
include(cmake/deps.cmake)

project(
  sdata
  DESCRIPTION "sdata language implementation written in modern C++ for fast data structures serialization"
  LANGUAGES CXX
)

add_subdirectory(include/sdata/)

if (${SDATA_BUILD_FORMAT})
  project(sdata_format)
  add_subdirectory(src/sdata_format/)
endif()

if(${SDATA_BUILD_TEST})
  enable_testing()
  project(sdata_test)
  add_subdirectory(test/)
endif()
