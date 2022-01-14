#ifndef SDATA_REGEX_PARSER
#define SDATA_REGEX_PARSER

#include "misc/exception.hpp"
#include "misc/fmt.hpp"
#include "regex_automata.hpp"
#include <deque>
#include <fmt/format.h>

namespace sdata {

class RegexParserException: public Exception {
public:
  RegexParserException(
    std::string_view description,
    std::string_view pattern,
    std::string_view::iterator token) :
    Exception(message(description, pattern, token)) {}

  static std::string
  message(std::string_view description, std::string_view pattern, std::string_view::iterator token);
};

class RegexParser {
public:
  explicit RegexParser(std::string_view pattern);
  RegexAutomata parse();

private:
  RegexAutomata merge_sequences();
  RegexAutomata parse_operand(std::string_view::iterator& token);
  auto parse_sequence_pattern(std::string_view::iterator& token);

  void parse_token(std::string_view::iterator& token);
  void parse_character_class(std::string_view::iterator& token);
  void parse_any(std::string_view::iterator& token);
  void parse_literal(std::string_view::iterator& token);
  void parse_sequence(std::string_view::iterator& token);
  void parse_alternative(std::string_view::iterator& token);
  void parse_quest(std::string_view::iterator& token);
  void parse_kleene(std::string_view::iterator& token);
  void parse_plus(std::string_view::iterator& token);
  void parse_wave(std::string_view::iterator& token);

  std::deque<RegexAutomata> m_stack;
  std::string_view m_pattern;
};

}  // namespace sdata

#endif
