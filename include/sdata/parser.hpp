#ifndef SDATA_PARSER_HPP
#define SDATA_PARSER_HPP

#include "misc/assert.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include <optional>

namespace sdata {

class ParserException : public CodeException {
public:
  ParserException(std::string_view description, const Token &token) :
    CodeException("sdata::ParserException", description, token) {}
};

class Parser {
public:
  explicit Parser(std::string_view source);

  inline Node parse() {
    return parse_node(false).value_or(Node {"", nullptr});
  }

private:
  std::optional<Node> parse_node(bool required);
  Variant parse_sequence();
  Variant parse_variant();
  Variant parse_array();

  Token parse_token(unsigned expected = Token::NONE);
  void throw_unexpected_token(const Token &token, unsigned expected);

  Scanner m_scanner;
};

}  // namespace sdata

#endif
