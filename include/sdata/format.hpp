#ifndef SDATA_FORMAT_HPP
#define SDATA_FORMAT_HPP

#include "node.hpp"
#include "serializer.hpp"

namespace sdata {

struct Format {
  /// Tokens
  std::string_view indent, assignment, separator, quote, container_end;

  /// Container bounds tokens
  struct Bounds {
    std::string_view open, close;
  } array_bounds, sequence_bounds, anonymous_bounds;

  /// Shifts the indentation of the anonymous content
  int anonymous_shift;

  static Format standard() {
    return {
      "  ",
      ": ",
      ",\n",
      "'",
      "\n",
      {" [\n", "]"},
      {" {\n", "}"},
      {"{\n", "\n}"},
      -1,
    };
  }

  static Format inlined() {
    return {
      "",
      ": ",
      ", ",
      "'",
      "",
      {" [ ", " ] "},
      {" { ", " } "},
      {" { ", " } "},
      0,
    };
  }

  static Format minimal() {
    return {
      "",
      ":",
      ",",
      "'",
      "",
      {"[", "]"},
      {"{", "}"},
      {"{", "}"},
      0,
    };
  }
};

template<>
struct Serializer<Format::Bounds> : Convert<Format::Bounds> {
  void decode(const Node &node, Format::Bounds &bounds) override {
    bounds = {node[0].get<std::string_view>(), node[1].get<std::string_view>()};
  }
};

template<>
struct Serializer<Format> : Convert<Format> {
  void decode(const Node &node, Format &format) override {
    format = {
      node.at("indent").get<std::string>(),
      node.at("assignment").get<std::string>(),
      node.at("separator").get<std::string>(),
      node.at("quote").get<std::string>(),
      node.at("container_end").get<std::string>(),
      node.at("array_bounds"),
      node.at("sequence_bounds"),
      node.at("anonymous_bounds"),
      node.at("anonymous_shift").get<int>(),
    };
  }
};

}  // namespace sdata

#endif
