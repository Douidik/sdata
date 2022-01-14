#include "scanner.hpp"

namespace sdata {

Scanner::Scanner(std::string_view source) : m_source(source), m_iter(m_source.begin()) {}

Token Scanner::tokenize() {
  Token token {{}, Token::NONE, {m_source, m_iter}};

  if (done()) {
    token.category = Token::DONE;
    return token;
  }

  for (const auto &[category, pattern] : Token::PATTERN) {
    if (auto match = pattern.match(m_iter, m_source.end())) {
      token.expression = {m_iter, m_iter += match.length};
      token.category = category;
      break;
    }
  }

  if (token.category & Token::NONE) {
    // Token unrecognized by scanner, split the token by space for debug
    token.expression = {m_iter, std::find(m_iter, m_source.end(), ' ')};
    throw ScannerException {"Unrecognized token", token};
  }

  // skip comments and empty tokens
  return !(token.category & Token::SKIP) ? token : tokenize();
}

}  // namespace sdata
