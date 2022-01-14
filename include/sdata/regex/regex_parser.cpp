#include "regex_parser.hpp"
#include "misc/trim.hpp"
#include "regex_category.hpp"
#include <unordered_map>

namespace sdata {

std::string RegexParserException::message(
  std::string_view description,
  std::string_view pattern,
  std::string_view::iterator token) {
  constexpr std::string_view PATTERN =
    "[sdata::RegexParserException raised]: {}\n"
    "with {{\n"
    "\t\"{}\" at [{}]: '{}'\n"
    "}}";

  return fmt(PATTERN, description, pattern, token - pattern.begin(), *token);
}

RegexParser::RegexParser(std::string_view pattern) : m_pattern(trim(pattern, REGEX_TOKEN_SPACE)) {}

RegexAutomata RegexParser::parse() {
  for (auto token = m_pattern.begin(); token != m_pattern.end(); token++) {
    parse_token(token);
  }

  return m_stack.empty() ? RegexAutomata {} : merge_sequences();
}

RegexAutomata RegexParser::merge_sequences() {
  RegexAutomata &automata = m_stack.front();

  for (size_t i = 1; i < m_stack.size(); i++) {
    automata.merge(m_stack[i], automata.leaves());
  }

  return automata;
}

RegexAutomata RegexParser::parse_operand(std::string_view::iterator &token) {
  if (m_stack.empty() || m_stack.back().empty()) {
    throw RegexParserException {
      "Preceding sequence is unquantifiable or missing",
      m_pattern,
      token,
    };
  }

  auto operand = m_stack.back();
  m_stack.pop_back();
  return operand;
}

auto RegexParser::parse_sequence_pattern(std::string_view::iterator &token) {
  auto begin = token + 1;
  size_t depth = 1;

  auto end = std::find_if(begin, m_pattern.end(), [&depth](auto sequence_token) {
    switch (sequence_token) {
      case REGEX_TOKEN_BEG_SEQ: depth++; break;
      case REGEX_TOKEN_END_SEQ: depth--; break;
    }
    return depth < 1;
  });

  if (end == m_pattern.end()) {
    throw RegexParserException {
      "Unterminated sequence, missing '}' closing operator",
      m_pattern,
      token,
    };
  }

  return std::make_pair(begin, end);
}

void RegexParser::parse_token(std::string_view::iterator &token) {
  // Out of range tokens
  if (token == m_pattern.end()) {
    return;
  }

  switch (*token) {
    case REGEX_TOKEN_SPACE: token++; return parse_token(token);
    case REGEX_TOKEN_BLANK:
    case REGEX_TOKEN_ALPHA:
    case REGEX_TOKEN_OPERATOR:
    case REGEX_TOKEN_NUMBER:
    case REGEX_TOKEN_QUOTE:
    case REGEX_TOKEN_APOSTROPHE: return parse_character_class(token);
    case REGEX_TOKEN_ANY: return parse_any(token);
    case REGEX_TOKEN_LITERAL: return parse_literal(token);
    case REGEX_TOKEN_BEG_SEQ: return parse_sequence(token);
    case REGEX_TOKEN_ALTERNATIVE: return parse_alternative(token);
    case REGEX_TOKEN_PLUS: return parse_plus(token);
    case REGEX_TOKEN_QUEST: return parse_quest(token);
    case REGEX_TOKEN_KLEENE: return parse_kleene(token);
    case REGEX_TOKEN_WAVE: return parse_wave(token);

    case REGEX_TOKEN_END_SEQ:
      throw RegexParserException {
        "Unexpected sequence end, missing '{' opening character",
        m_pattern,
        token,
      };

    default:
      throw RegexParserException {
        "Unrecognized token in pattern",
        m_pattern,
        token,
      };
  }
}

void RegexParser::parse_character_class(std::string_view::iterator &token) {
  //      -> first_character_alternative
  // root -> ...
  //      -> last_character_alternative

  static const std::unordered_map<char, std::string_view> s_character_map {
    {REGEX_TOKEN_BLANK, "\n\t\v\b\f "},
    {REGEX_TOKEN_ALPHA, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"},
    {REGEX_TOKEN_OPERATOR, "!#$%&()*+,-./:;<=>?@[\\]^`{|}~"},
    {REGEX_TOKEN_NUMBER, "0123456789"},
    {REGEX_TOKEN_QUOTE, "\""},
    {REGEX_TOKEN_APOSTROPHE, "'"},
  };

  auto &sequence = m_stack.emplace_back();
  RegexNode *root = sequence.insert(REGEX_EPSILON, {}, {});
  std::string_view map = s_character_map.at(*token);

  for (const char &c : map) {
    sequence.insert(&c, {root}, {});
  }
}

void RegexParser::parse_any(std::string_view::iterator &token) {
  m_stack.emplace_back().insert(REGEX_ANY, {}, {});
}

void RegexParser::parse_literal(std::string_view::iterator &token) {
  // first_character -> ... -> last_character

  auto begin = token + 1, end = std::find(begin, m_pattern.end(), REGEX_TOKEN_LITERAL);

  if (end == m_pattern.end()) {
    throw RegexParserException {
      "Unterminated string literal, missing closing character",
      m_pattern,
      token,
    };
  }

  auto &sequence = m_stack.emplace_back();
  RegexNode *node = sequence.insert(&*begin, {}, {});

  for (const char &c : std::string_view {begin + 1, end}) {
    node = sequence.insert(&c, {node}, {});
  }

  token = end;
}

void RegexParser::parse_sequence(std::string_view::iterator &token) {
  auto [begin, end] = parse_sequence_pattern(token);
  m_stack.emplace_back(RegexParser({begin, end}).parse());
  token = end;
}

void RegexParser::parse_alternative(std::string_view::iterator &token) {
  // root -> first_alternative
  //      -> second_alternative

  RegexAutomata sequence {};
  RegexNode *root = sequence.insert(REGEX_EPSILON, {}, {});

  if (m_stack.empty() || m_stack.back().empty()) {
    throw RegexParserException {"Missing left alternative", m_pattern, token};
  } else {
    sequence.merge(m_stack.back(), {root});
    m_stack.pop_back();
  }

  if (parse_token(++token); m_stack.empty() || m_stack.back().empty()) {
    throw RegexParserException {"Missing right alternative", m_pattern, token};
  } else {
    sequence.merge(m_stack.back(), {root});
    m_stack.pop_back();
  }

  m_stack.push_back(sequence);
}

void RegexParser::parse_quest(std::string_view::iterator &token) {
  // root -> operand -> next
  //      -> epsilon -> next

  auto operand = parse_operand(token);
  auto &sequence = m_stack.emplace_back();
  RegexNode *root = sequence.insert(REGEX_EPSILON, {}, {});
  RegexNode *epsilon = sequence.insert(REGEX_EPSILON, {root}, {});
  sequence.merge(operand, {root});
}

void RegexParser::parse_kleene(std::string_view::iterator &token) {
  // root -> operand -> root
  //      -> epsilon -> next

  auto operand = parse_operand(token);
  auto &sequence = m_stack.emplace_back();
  RegexNode *root = sequence.insert(REGEX_EPSILON, {}, {});
  RegexNode *op_root = sequence.merge(operand, {root});
  RegexNode *epsilon = sequence.insert(REGEX_EPSILON, {root}, {});

  for (auto leaves = op_root->leaves(); RegexNode * leaf : leaves) {
    leaf->edges.insert(root);
  }
}

void RegexParser::parse_plus(std::string_view::iterator &token) {
  // operand -> epsilon -> operand
  //                    -> next

  auto operand = parse_operand(token);
  operand.insert(REGEX_EPSILON, operand.leaves(), {operand.root()});
  m_stack.emplace_back(operand);
}

void RegexParser::parse_wave(std::string_view::iterator &token) {
  // root -> operand -> next
  //      -> any     -> root

  if (parse_token(++token); m_stack.empty() || m_stack.back().empty()) {
    throw RegexParserException {
      "Wave delimiter is missing or unquantifiable",
      m_pattern,
      token,
    };
  }

  auto operand = m_stack.back();
  m_stack.pop_back();

  auto &sequence = m_stack.emplace_back();
  RegexNode *root = sequence.insert(REGEX_EPSILON, {}, {});
  RegexNode *op_root = sequence.merge(operand, {root});
  RegexNode *any = sequence.insert(REGEX_ANY, {root}, {root});
}

}  // namespace sdata
