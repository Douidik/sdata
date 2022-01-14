#ifndef SDATA_BIT_HPP
#define SDATA_BIT_HPP

namespace sdata {

inline constexpr auto bit(auto n) -> decltype(n) {
  return 1 << n;
}

}  // namespace sdata

#endif
