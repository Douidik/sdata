#ifndef SDATA_FMT_HPP
#define SDATA_FMT_HPP

#include <fmt/format.h>

namespace sdata {

auto fmt(std::string_view pattern, auto... args) {
  return fmt::vformat(pattern, fmt::make_format_args(args...));
}

}  // namespace sdata

#endif
