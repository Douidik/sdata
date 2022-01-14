#ifndef SDATA_FORMAT_HPP
#define SDATA_FORMAT_HPP

#include "node.hpp"
#include "serializer.hpp"

namespace sdata {

struct Format {
  /// Tokens
  std::string indent, assignment, separator, quote, container_end;
  /// Container token bounds
  std::string array_bounds[2], sequence_bounds[2], anonymous_bounds[2];
  /// Shifts the indentation of the anonymous content
  int anonymous_shift;

  static Format classic() {
    return {
      "\t",
      ": ",
      ",\n",
      "'",
      "\n",
      {" [\n", "]"},
      {" {\n", "}"},
      {"{\n\n", "\n}"},
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
      {" [", "]"},
      {" {", "}"},
      {"{", "}"},
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
struct Serializer<Format> : std::true_type {
  void encode(Node &node, const Format &format) {
    node = Sequence {
      {"indent", format.indent},
      {"assignment", format.assignment},
      {"separator", format.separator},
      {"quote", format.quote},
      {"container_end", format.container_end},
      {"array_bounds", Array {format.array_bounds, std::end(format.array_bounds)}},
      {"sequence_bounds", Array {format.sequence_bounds, std::end(format.sequence_bounds)}},
      {"anonymous_bounds", Array {format.anonymous_bounds, std::end(format.anonymous_bounds)}},
      {"anonymous_shift", format.anonymous_shift},
    };
  }

  void decode(const Node &node, Format &format) {
    const auto &array_bounds = node.at("array_bounds").get<Array>();
    const auto &sequence_bounds = node.at("sequence_bounds").get<Array>();
    const auto &anonymous_bounds = node.at("anonymous_bounds").get<Array>();

    format = Format {
      node.at("indent").get<std::string>(),
      node.at("assignment").get<std::string>(),
      node.at("separator").get<std::string>(),
      node.at("quote").get<std::string>(),
      node.at("container_end").get<std::string>(),
      {
        array_bounds.at(0).get<std::string>(),
        array_bounds.at(1).get<std::string>(),
      },
      {
        sequence_bounds.at(0).get<std::string>(),
        sequence_bounds.at(1).get<std::string>(),
      },
      {
        anonymous_bounds.at(0).get<std::string>(),
        anonymous_bounds.at(1).get<std::string>(),
      },
      node.at("anonymous_shift").get<int>(),
    };
  };
};

}  // namespace sdata

#endif
