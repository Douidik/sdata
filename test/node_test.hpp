#ifndef SDATA_NODE_TEST_HPP
#define SDATA_NODE_TEST_HPP

#include <catch2/catch.hpp>
#include <sdata/sdata.hpp>

namespace nested {

constexpr std::string_view SOURCE = "s{s_{s__:'hello'}}";

struct S__ {
  std::string_view name;
};

struct S_ {
  S__ nested;
};

struct S {
  S_ nested;
};

}  // namespace nested

namespace sdata {
using namespace nested;

template<>
struct Serializer<S__> : Scheme<S__(std::string_view)> {
  Map map(S__ &s__) {
    return Map {{"name", s__.name}};
  }
};

template<>
struct Serializer<S_> : Scheme<S_(S__)> {
  Map map(S_ &s_) {
    return Map {{"s__", s_.nested}};
  }
};

template<>
struct Serializer<S> : Scheme<S(S_)> {
  Map map(S &s) {
    return Map {{"s_", s.nested}};
  }
};

}  // namespace sdata

using namespace sdata;

TEST_CASE("Node: nested") {
  S s = parse_str(nested::SOURCE);
  CHECK(s.nested.nested.name == "hello");
}

#endif
