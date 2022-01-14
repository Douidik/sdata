#ifndef SDATA_PARSER_HPP
#define SDATA_PARSER_HPP

#include "misc/assert.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include <charconv>

namespace sdata {

class ParserException : public CodeException {
public:
  ParserException(std::string_view description, const Token &token) :
    CodeException("sdata::ParserException", description, token) {}
};

class Parser {
public:
  explicit Parser(std::string_view source);
  Node parse();

private:
  void parse_node(Node &owner);
  void parse_sequence(Node &owner);
  Variant parse_variant();
  Variant parse_array();

  Token parse_token(unsigned expected = Token::NONE);
  void throw_unexpected_token(const Token &token, unsigned expected);

  template<typename T>
  Variant parse_number(Token token) {
    std::string_view str = token.expression;
    Variant v {};

    const auto [ptr, status] = std::from_chars(str.data(), str.data() + str.size(), v.as<T>());

    if (status != std::errc()) {
      throw ParserException {fmt("std::from_chars failed to parse number: '{}'", str), token};
    }

    return v;
  }

  Scanner m_scanner;
};

}  // namespace sdata

#endif
