#ifndef SDATA_TRAITS_HPP
#define SDATA_TRAITS_HPP

#include "misc/any_of.hpp"
#include "serializer.hpp"
#include <iostream>

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

template<typename T>
struct _Traits;

template<typename T>
requires(std::same_as<T, std::nullptr_t>) struct _Traits<T> {
  constexpr static std::size_t index = NIL;
};

template<typename T>
requires(std::same_as<T, class Array>) struct _Traits<T> {
  constexpr static std::size_t index = Type::ARRAY;
};

template<typename T>
requires(std::same_as<T, class Sequence>) struct _Traits<T> {
  constexpr static std::size_t index = Type::SEQUENCE;
};

template<typename T>
requires(std::is_floating_point_v<T>) struct _Traits<T> {
  constexpr static std::size_t index = Type::FLOAT;
};

template<typename T>
requires(!std::same_as<T, bool> && std::is_integral_v<T>) struct _Traits<T> {
  constexpr static std::size_t index = Type::INT;
};

template<typename T>
requires(std::same_as<T, bool>) struct _Traits<T> {
  constexpr static std::size_t index = Type::BOOL;
};

template<typename T>
requires(any_of<T, std::string, std::string_view, const char *, char *>) struct _Traits<T> {
  constexpr static std::size_t index = Type::STRING;
};

template<typename T>
struct Traits : _Traits<std::decay_t<T>> {};

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
