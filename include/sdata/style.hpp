#ifndef SDATA_STYLE_HPP
#define SDATA_STYLE_HPP

#include "misc/bit.hpp"
#include "serializer.hpp"
#include <string_view>

namespace sdata {

struct Style {
  std::string indent, assignment, separator, quote, left_bracket, right_bracket, left_brace,
    right_brace;

  static Style classic() {
    return {
      "  ",
      ": ",
      ",\n",
      "'",

      " [\n",
      "]\n",

      " {\n",
      "}\n",
    };
  }

  static Style inlined() {
    return {
      "",
      ": ",
      ", ",
      "'",

      " [",
      "]",

      " {",
      "}",
    };
  }

  static Style minimal() {
    return {"", ":", ",", "'", "[", "]", "{", "}"};
  }
};

template<>
class Serializer<Style> : std::true_type {
  void encode(Node& node, const Style& style) {
    node = Sequence {
      {"indent", style.indent},
      {"assignment", style.assignment},
      {"separator", style.separator},
      {"left_bracket", style.left_bracket},
      {"right_bracket", style.right_bracket},
      {"left_brace", style.left_brace},
      {"right_brace", style.right_brace},
    };
  }

  void decode(Node& node, Style& style) {
    auto classic = Style::classic();

    style = {
      node["indent"].as<std::string>(classic.indent),
      node["assignment"].as<std::string>(classic.assignment),
      node["separator"].as<std::string>(classic.separator),
      node["left_bracket"].as<std::string>(classic.left_bracket),
      node["right_bracket"].as<std::string>(classic.right_bracket),
      node["left_brace"].as<std::string>(classic.left_brace),
      node["right_brace"].as<std::string>(classic.right_brace),
    };
  }
};

};  // namespace sdata

#endif
