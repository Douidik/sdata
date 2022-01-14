#ifndef SDATA_SOURCE_LOCATION_HPP
#define SDATA_SOURCE_LOCATION_HPP

#include "trim.hpp"
#include <algorithm>
#include <string_view>

namespace sdata {

struct SourceLocation {
public:
  SourceLocation(std::string_view source, std::string_view::iterator iterator) :
    source(source),
    index(std::distance(source.cbegin(), iterator)),
    line(std::count(source.cbegin(), iterator, '\n') + 1) {}

  SourceLocation() : source {}, index((size_t)-1), line((size_t)-1) {}

  constexpr inline std::string_view snippet() const {
    if (index >= source.size()) {
      return {};
    }

    std::string_view s = {
      source.begin() + source.rfind('\n', index) + 1,
      source.begin() + source.find('\n', index),
    };

    return trim(s, ' ');
  }

  std::string_view source;
  size_t index, line;
};

};  // namespace sdata

#endif
