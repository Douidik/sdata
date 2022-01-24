#ifndef SDATA_REGEX_MATCH_HPP
#define SDATA_REGEX_MATCH_HPP

#include <utility>

namespace sdata {

struct RegexMatch {
  inline operator bool() const {
    return found;
  }

  bool found;
  std::size_t length;
};

}  // namespace sdata

#endif
