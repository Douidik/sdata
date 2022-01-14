#include "writer.hpp"

namespace sdata {

Writer::Writer(const Node &node, Format format) : m_root(node), m_format(format) {
  write_node(m_root, 0);
}

void Writer::write_node(const Node &node, int depth) {
  if (node.is_anonymous()) {
    return write_container(
      node.get<Sequence>(),
      &Writer::write_node,
      m_format.anonymous_bounds,
      depth + m_format.anonymous_shift);
  }

  write("{}", node.id());

  if (!node.is<Sequence>()) {
    write("{}", m_format.assignment);
  }

  write_variant(node, depth);
}

void Writer::write_variant(const Variant &variant, int depth) {
  switch (variant.type()) {
    case ARRAY: {
      const auto &array = variant.get<Array>();
      return write_container(array, &Writer::write_variant, m_format.array_bounds, depth);
    }

    case SEQUENCE: {
      const auto &sequence = variant.get<Sequence>();
      return write_container(sequence, &Writer::write_node, m_format.sequence_bounds, depth);
    }

    case NIL: {
      return write("nil");
    }

    case FLOAT: {
      return write("{:f}", variant.get<float>());
    }

    case INT: {
      return write("{:d}", variant.get<int>());
    }

    case BOOL: {
      return write("{}", variant.get<bool>());
    }

    case STRING: {
      return write("{}{:s}{}", m_format.quote, variant.get<std::string>(), m_format.quote);
    }
  }
}

void Writer::write_indent(int depth) {
  for (int i = 0; i < depth; i++) {
    write("{}", m_format.indent);
  }
}

}  // namespace sdata
