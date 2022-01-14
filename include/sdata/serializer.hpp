#ifndef SDATA_SERIALIZER_HPP
#define SDATA_SERIALIZER_HPP

#include <type_traits>

namespace sdata {

template<typename T>
struct Serializer : std::false_type {
  void encode(class Node &node, const T &);
  void decode(const class Node &node, T &);
};

}  // namespace sdata

#endif
