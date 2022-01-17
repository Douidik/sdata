#ifndef SDATA_PARSE_NUMBER_HPP
#define SDATA_PARSE_NUMBER_HPP

#include "exception.hpp"
#include "fmt.hpp"
#include <charconv>

namespace sdata {

template<typename T>
static T parse_number(std::string_view str) {
  T number {};
  auto [ptr, status] = std::from_chars(str.begin(), str.begin() + str.size(), number);

  if (status != std::errc {}) {
    throw Exception {fmt("[sdata::Exception raised] number parsing failed: '{}'", str)};
  } else {
    return number;
  }
}

}  // namespace sdata

#endif
