file(GLOB_RECURSE SDATA_TEST_SOURCE ${SDATA_SOURCE_FILE_REGEX}*.hpp ${SDATA_SOURCE_FILE_REGEX}*.cpp)
add_executable(sdata_test ${SDATA_TEST_SOURCE})

target_include_directories(sdata_test PRIVATE ${SDATA_ROOT}/src/ ${SDATA_ROOT}/test/)
target_link_libraries(sdata_test PRIVATE sdata Catch2::Catch2WithMain)

set_target_properties(
  sdata_test PROPERTIES
  BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
  
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  LINKER_LANGUAGE CXX
)
