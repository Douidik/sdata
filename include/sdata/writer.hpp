#ifndef SDATA_WRITER_HPP
#define SDATA_WRITER_HPP

#include "format.hpp"
#include "misc/basic_writer.hpp"

namespace sdata {

class Writer : public BasicWriter {
public:
  explicit Writer(const Node &node, Format format = Format::standard());
  explicit Writer(const Variant &variant, Format format = Format::inlined());

private:
  void write_node(const Node &node, int depth);
  void write_variant(const Variant &variant, int depth);
  void write_indent(int depth);

  void write_container(const auto &container, auto write_fn, Format::Bounds bounds, int depth) {
    write("{}", bounds.open);

    for (size_t i = 0; i < container.size(); i++) {
      write_indent(depth + 1);
      (this->*write_fn)(container[i], depth + 1);

      if (i < container.size() - 1) {
        write("{}", m_format.separator);
      } else {
        write("{}", m_format.container_end);
      }
    }

    write_indent(depth);
    write("{}", bounds.close);
  }

  Format m_format;
};

inline std::ostream &operator<<(std::ostream &os, Node &node) {
  return os << Writer {node}.buffer();
}

}  // namespace sdata

#endif
