#ifndef SDATA_SERIALIZER_HPP
#define SDATA_SERIALIZER_HPP

#include "node.hpp"

namespace sdata {

template<typename T>
class Serializer {
public:
  void encode(Node &node, const T &) {
    throw Exception {"Serializer::encode has no specialization for type T"};
  }
  void decode(const Node &node, T &) {
    throw Exception {"Serializer::decode has no specialization for type T"};
  }
};

}  // namespace sdata

#endif
