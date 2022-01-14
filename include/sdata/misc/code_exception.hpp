#ifndef SDATA_CODE_EXCEPTION_HPP
#define SDATA_CODE_EXCEPTION_HPP

#include "exception.hpp"
#include "misc/fmt.hpp"
#include "token.hpp"

namespace sdata {

class CodeException : public Exception {
public:
  CodeException(std::string_view name, std::string_view description, const Token &token) :
    Exception(message(name, description, token)) {}

private:
  static std::string
  message(std::string_view name, std::string_view description, const Token &token) {
    constexpr std::string_view PATTERN =
      "[{} raised]: {}\n"
      "with {{\n"
      "\t{} | {}\n"
      "\t{}\n"
      "}}";

    return fmt(
      PATTERN,
      name,
      description,
      token.source_location.line,
      token.source_location.snippet(),
      token);
  }
};

}  // namespace sdata

#endif
