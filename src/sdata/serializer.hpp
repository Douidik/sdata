#ifndef SDATA_SERIALIZER_HPP
#define SDATA_SERIALIZER_HPP

#include "node.hpp"
#include <unordered_map>

namespace sdata {

template<typename T>
class Serializer {
public:
  virtual void encode(Node &node, const T &) {
    throw Exception {"Serializer::encode has no specialization for type T"};
  }
  virtual void decode(const Node &node, T &) {
    throw Exception {"Serializer::decode has no specialization for type T"};
  }
};

}  // namespace sdata

#endif
