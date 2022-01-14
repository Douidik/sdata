#ifndef SDATA_EXCEPTION_HPP
#define SDATA_EXCEPTION_HPP

#include <exception>
#include <string>
#include <string_view>

namespace sdata {

class Exception : public std::exception {
public:
  Exception(std::string_view message) : m_buffer(message) {}

  inline const char *what() const noexcept override {
    return m_buffer.data();
  }

private:
  std::string m_buffer {};
};

}  // namespace sdata

#endif
