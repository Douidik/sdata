add_executable(sdata_format main.cpp)
target_include_directories(sdata_format PRIVATE ${SDATA_ROOT}/include/)
target_link_libraries(sdata_format PRIVATE sdata cxxopts::cxxopts)

set_target_properties(
  sdata_format PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  LINKER_LANGUAGE CXX
)
