#ifndef SDATA_REGEX_WRITER_HPP
#define SDATA_REGEX_WRITER_HPP

#include "misc/basic_writer.hpp"
#include <iostream>

namespace sdata {

class RegexWriter : public BasicWriter {
public:
  explicit RegexWriter(const class RegexAutomata &automata);

private:
  void write_graphviz();
  void write_root();
  void write_shapes();
  void write_edges();
  std::string_view parse_state(struct RegexNode *node);

  const class RegexAutomata &m_automata;
};

inline std::ostream &operator<<(std::ostream &os, const class RegexAutomata &automata) {
  return os << RegexWriter {automata}.buffer();
}

}  // namespace sdata

#endif
