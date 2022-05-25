option(SDATA_BUILD_TEST "build sdata's test suite" OFF)
option(SDATA_BUILD_FORMAT "build sdata format tool" ON)
option(SDATA_ASSERTIONS "enable inner library assertions" OFF)

set(SDATA_SOURCE_FILE_REGEX "[a-z_]")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDATA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
