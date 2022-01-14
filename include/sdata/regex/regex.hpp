#ifndef SDATA_REGEX_HPP
#define SDATA_REGEX_HPP

#include "regex_writer.hpp"
#include "regex_parser.hpp"

namespace sdata {

class Regex {
public:
  Regex(std::string_view pattern) : m_pattern(pattern), m_automata(RegexParser {pattern}.parse()) {}

  inline std::string_view pattern() const {
    return m_pattern;
  }

  inline const RegexAutomata& automata() const {
    return m_automata;
  }

  inline RegexMatch match(std::string_view expression) const {
    return match(expression.begin(), expression.end());
  }

  template<typename T>
  inline RegexMatch match(T begin, T end) const {
    return m_automata.run<T>(begin, begin, end, m_automata.root());
  }

private:
  RegexAutomata m_automata;
  std::string_view m_pattern;
};

namespace regex_literals {

  inline Regex operator""_re(const char* pattern, size_t) {
    return {pattern};
  }

}  // namespace regex_literals

}  // namespace sdata

#endif
