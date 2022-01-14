#ifndef SDATA_SCANNER_HPP
#define SDATA_SCANNER_HPP

#include "misc/code_exception.hpp"
#include "token.hpp"

namespace sdata {

class ScannerException : public CodeException {
public:
  ScannerException(std::string_view description, const Token &token) :
    CodeException("sdata::ScannerException", description, token) {}
};

class Scanner {
public:
  Scanner(std::string_view source);

  Token tokenize();

  inline std::string_view source() const {
    return m_source;
  }

  inline bool done() const {
    return m_iter == m_source.end();
  }

private:
  std::string_view m_source;
  std::string_view::iterator m_iter;
};

}  // namespace sdata

#endif
