file(GLOB_RECURSE SDATA_SOURCE ${SDATA_SOURCE_FILE_REGEX}*.hpp ${SDATA_SOURCE_FILE_REGEX}*.cpp)
add_library(sdata ${SDATA_SOURCE})

target_include_directories(sdata PUBLIC ${SDATA_ROOT}/src/sdata/)
target_link_libraries(sdata PUBLIC fmt::fmt)

set_target_properties(
  sdata PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/
  
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  LINKER_LANGUAGE CXX
)

if(${SDATA_ASSERTIONS})
  target_compile_definitions(sdata PRIVATE SDATA_ASSERTIONS)
endif()
