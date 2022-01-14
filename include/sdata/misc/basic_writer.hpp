#ifndef SDATA_BASIC_WRITER_HPP
#define SDATA_BASIC_WRITER_HPP

#include <fmt/core.h>
#include <string>

namespace sdata {

class BasicWriter {
public:
  inline std::string_view buffer() const {
    return m_buffer;
  }

protected:
  inline void write(std::string_view pattern, auto... args) {
    fmt::vformat_to(std::back_inserter(m_buffer), pattern, fmt::make_format_args(args...));
  }

  std::string m_buffer {};
};

}  // namespace sdata

#endif
