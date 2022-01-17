#ifndef SDATA_ESCAPED_HPP
#define SDATA_ESCAPED_HPP

#include <istream>

namespace sdata {

constexpr char escape_sequence(char code) {
  switch (code) {
    case '?': return '\?';
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    case '\\': return '\\';
    default: return code;  // unrecognized escape sequence
  }
}

struct Escaped {
  std::istream &stream;
};

static std::ostream &operator<<(std::ostream &os, const Escaped &escaped) {
  for (std::istreambuf_iterator<char> iter {escaped.stream}, end; iter != end; iter++) {
    os << (*iter != '\\' ? *iter : escape_sequence(*++iter));
  }

  return os;
}

}  // namespace sdata

#endif
