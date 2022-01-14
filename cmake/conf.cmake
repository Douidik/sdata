option(SDATA_BUILD_TEST "build sdata's test suite" OFF)
option(SDATA_BUILD_FORMAT "build sdata format tool" ON)
option(SDATA_BUILD_EXAMPLES "build sdata examples")
option(SDATA_ASSERTIONS "enable inner library assertions" ON)

set(SDATA_SOURCE_FILE_REGEX "[a-z_]")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDATA_ROOT ${CMAKE_SOURCE_DIR})
