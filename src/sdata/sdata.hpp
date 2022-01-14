#ifndef SDATA_HPP
#define SDATA_HPP

#include "parser.hpp"
#include "writer.hpp"
#include <filesystem>
#include <fstream>

namespace sdata {

static std::string read_file(std::filesystem::path path) {
  std::ifstream fstream {path, std::ios::in};

  if (!fstream.is_open()) {
    throw Exception {fmt("Can't read source from: '{}'", path.string())};
  }

  return {std::istreambuf_iterator(fstream), {}};
}

inline Node parse_str(std::string_view source) {
  return Parser(source).parse();
}

inline Node parse_file(std::filesystem::path path) {
  return parse_str(read_file(path));
}

inline std::string write_str(const Node &node, Format format = Format::classic()) {
  return std::string {Writer(node, format).buffer()};
}

inline void
write_file(std::filesystem::path path, const Node &node, Format format = Format::classic()) {
  std::ofstream fstream {path, std::ios::out};
  fstream << Writer(node, format).buffer();
}

}  // namespace sdata

namespace sdata::literals {

inline Node operator""_sdata(const char *source, size_t) {
  return parse_str(source);
}

}  // namespace sdata::literals

#endif
