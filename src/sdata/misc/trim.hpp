#ifndef SDATA_TRIM_HPP
#define SDATA_TRIM_HPP

#include <string_view>

namespace sdata {

constexpr std::string_view trim(std::string_view sv, char c) {
  sv.remove_prefix(std::min(sv.find_first_not_of(c), sv.size()));
  sv.remove_suffix(std::min(sv.size() - sv.find_last_not_of(c) - 1, sv.size()));

  return sv;
}

}  // namespace sdata

#endif
