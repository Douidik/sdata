#ifndef SDATA_TOKEN_HPP
#define SDATA_TOKEN_HPP

#include "misc/bit.hpp"
#include "misc/source_location.hpp"
#include "regex/regex.hpp"
#include <fmt/format.h>
#include <string_view>
#include <unordered_map>

namespace sdata {

struct Token {
  enum Category {
    NONE,

    ID = bit(0),
    SET = bit(1),
    SEPARATOR = bit(2),
    BEG_SEQ = bit(3),
    END_SEQ = bit(4),
    ASSIGNMENT = SET | BEG_SEQ,

    FLOAT = bit(5),
    INT = bit(6),
    TRUE = bit(7),
    FALSE = bit(8),
    STRING = bit(9),
    BEG_ARR = bit(10),
    END_ARR = bit(11),
    NIL = bit(12),
    DATA = FLOAT | INT | TRUE | FALSE | STRING | BEG_ARR | NIL,


    EMPTY = bit(13),
    COMMENT = bit(14),
    DONE = bit(15),
    SKIP = EMPTY | COMMENT,

    CATEGORY_COUNT = 16,
  };

  inline static const std::unordered_map<Category, Regex> PATTERN {
    {COMMENT, {" '#'~'#' "}},
    {FALSE, {" 'false' "}},
    {TRUE, {" 'true' "}},
    {SEPARATOR, {" ',' "}},
    {END_SEQ, {" '}' "}},
    {BEG_SEQ, {" '{' "}},
    {END_ARR, {" ']' "}},
    {BEG_ARR, {" '[' "}},
    {SET, {" ':' "}},
    {NIL, {" 'nil' "}},
    {ID, {"{a|'_'} {a|n|'_'}*"}},
    {INT, {"{'-'|'+'}? n+"}},
    {FLOAT, {"{'-'|'+'}? n+ '.' n+ 'f'?"}},
    {STRING, {"{q~q}|{Q~Q}"}},
    {EMPTY, {"_+"}},
  };

  std::string_view expression;
  Category category;
  SourceLocation source_location;
};

inline std::ostream &operator<<(std::ostream &os, const Token::Category &category) {
  switch (category) {
    using enum Token::Category;

    case ID: return os << "id";
    case SET: return os << "assign";
    case SEPARATOR: return os << "separator";
    case FLOAT: return os << "float";
    case INT: return os << "int";
    case TRUE: return os << "true";
    case FALSE: return os << "false";
    case STRING: return os << "string";
    case NIL: return os << "nil";
    case BEG_SEQ: return os << "beg_seq";
    case END_SEQ: return os << "end_seq";
    case BEG_ARR: return os << "beg_arr";
    case END_ARR: return os << "end_arr";
    case EMPTY: return os << "empty";
    case DONE: return os << "done";
    case NONE: return os << "none";
    default: return os << "?";
  }
}

}  // namespace sdata

template<>
struct fmt::formatter<sdata::Token> {
  inline constexpr auto parse(format_parse_context &context) {
    return context.begin();
  }

  template<typename F>
  auto format(const sdata::Token &token, F &context) {
    return format_to(context.out(), "Token<{}>: '{}'", token.category, token.expression);
  }
};

#endif
