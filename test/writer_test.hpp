#ifndef SDATA_WRITER_TEST_HPP
#define SDATA_WRITER_TEST_HPP

#include <catch2/catch.hpp>
#include <sdata/sdata.hpp>

using namespace sdata;

static bool test_writer(std::filesystem::path path) {
  auto expected = sdata::parse_file(path);
  auto written = sdata::write_str(expected);
  auto result = sdata::parse_str(written);
  return result == expected;
}

TEST_CASE("Writer") {
  CHECK(test_writer("examples/game.sd"));
  CHECK(test_writer("examples/dialog.sd"));
  CHECK(test_writer("examples/features.sd"));
}

#endif
