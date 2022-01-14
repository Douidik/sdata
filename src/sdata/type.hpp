#ifndef SDATA_TYPE_HPP
#define SDATA_TYPE_HPP

#include "misc/fmt.hpp"

namespace sdata {

enum Type {
  NIL,
  ARRAY,
  SEQUENCE,
  FLOAT,
  INT,
  BOOL,
  STRING,
};

inline std::ostream &operator<<(std::ostream &os, Type type) {
  switch (type) {
    case NIL: return os << "nil";
    case SEQUENCE: return os << "sequence";
    case ARRAY: return os << "array";
    case FLOAT: return os << "float";
    case INT: return os << "int";
    case BOOL: return os << "bool";
    case STRING: return os << "string";
    default: return os << "?";
  }
}

}  // namespace sdata

#endif
