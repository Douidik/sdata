#ifndef SDATA_REGEX_MATCH_HPP
#define SDATA_REGEX_MATCH_HPP

#include <cstddef>
#include <utility>

namespace sdata {

struct RegexMatch {
  inline operator bool() const {
    return matched;
  }

  bool matched;
  size_t length;
};

}  // namespace sdata

#endif
