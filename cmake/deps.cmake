include(FetchContent)

if(${SDATA_BUILD_TEST})
  fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.8
  )

  fetchcontent_makeavailable(Catch2)
endif()

if(${SDATA_BUILD_FORMAT})
  fetchcontent_declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.0.0
  )

  fetchcontent_makeavailable(cxxopts)
endif()

fetchcontent_declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)

fetchcontent_makeavailable(fmt)
