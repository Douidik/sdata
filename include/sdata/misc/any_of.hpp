#ifndef SDATA_ANY_OF_HPP
#define SDATA_ANY_OF_HPP

#include <concepts>

namespace sdata {

template<typename T, typename... Ts>
concept any_of = (std::same_as<T, Ts> || ...);

}

#endif
