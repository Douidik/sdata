#include "parser.hpp"
#include "misc/parse_number.hpp"
#include "misc/trim.hpp"

namespace sdata {

Parser::Parser(std::string_view source) : m_scanner(source) {}

std::optional<Node> Parser::parse_node(bool required) {
  Token token = required ? parse_token(Token::ID | Token::BEG_SEQ | Token::DONE)
                         : parse_token(Token::ID | Token::BEG_SEQ);

  // No token available
  if (token.category & Token::DONE) {
    return std::nullopt;
  }

  // Anonymous node
  if (token.category & Token::BEG_SEQ) {
    return Node {"", parse_sequence()};
  }

  Node node {token.expression, nullptr};
  Token assignment = parse_token(Token::ASSIGNMENT);

  if (assignment.category & Token::SET) {
    node = parse_variant();
  }
  if (assignment.category & Token::BEG_SEQ) {
    node = parse_sequence();
  }

  return node;
}

Variant Parser::parse_sequence() {
  Sequence sequence {};

  do {
    sequence.push_back(*parse_node(true));
  } while (parse_token(Token::SEPARATOR | Token::END_SEQ).category != Token::END_SEQ);

  return {sequence};
}

Variant Parser::parse_variant() {
  Token token = parse_token(Token::DATA);

  switch (token.category) {
    case Token::BEG_ARR: {
      return parse_array();
    }

    case Token::FLOAT: {
      return {parse_number<float>(token.expression)};
    }

    case Token::INT: {
      return {parse_number<int>(token.expression)};
    }

    case Token::TRUE: {
      return {true};
    }

    case Token::FALSE: {
      return {false};
    }

    case Token::STRING: {
      // Trim the quotes from the content
      std::string_view content = token.expression;
      content.remove_prefix(1);
      content.remove_suffix(1);

      return {std::string(content)};
    };

    default: return {nullptr};
  }
}

Variant Parser::parse_array() {
  Array array {};

  do {
    array.push_back(parse_variant());
  } while (parse_token(Token::SEPARATOR | Token::END_ARR).category != Token::END_ARR);

  return {array};
}

Token Parser::parse_token(unsigned expected) {
  Token token = m_scanner.tokenize();

  if (!(expected & token.category)) {
    throw_unexpected_token(token, expected);
  }

  return token;
}

void Parser::throw_unexpected_token(const Token &token, unsigned expected) {
  std::vector<std::string> expected_str {};

  for (unsigned i = 0; i < Token::CATEGORY_COUNT; i++) {
    if (bit(i) & expected) {
      auto str = fmt("{}", static_cast<Token::Category>(bit(i)));
      expected_str.push_back(str);
    }
  }

  throw ParserException {
    fmt("Expected token of type [{}]", fmt::join(expected_str, "/")),
    token,
  };
}

}  // namespace sdata
