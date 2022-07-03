#include "writer.hpp"

namespace sdata {

Writer::Writer(const Node &node, Format format) : m_format(format) {
  write_node(node, 0);
}

Writer::Writer(const Variant &variant, Format format) : m_format(format) {
  write_variant(variant, 0);
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

void Writer::write_variant(const Variant &node, int depth) {
  switch (node.type()) {
    case ARRAY: {
      const auto &array = node.get<Array>();
      return write_container(array, &Writer::write_variant, m_format.array_bounds, depth);
    }

    case SEQUENCE: {
      const auto &sequence = node.get<Sequence>();
      return write_container(sequence, &Writer::write_node, m_format.sequence_bounds, depth);
    }

    case NIL: {
      return write("nil");
    }

    case FLOAT: {
      return write("{:f}", node.get<float>());
    }

    case INT: {
      return write("{:d}", node.get<int>());
    }

    case BOOL: {
      return write("{}", node.get<bool>());
    }

    case STRING: {
      return write("{}{:s}{}", m_format.quote, node.get<std::string>(), m_format.quote);
    }

    default: return;
  }
}

void Writer::write_indent(int depth) {
  for (int i = 0; i < (m_format.flat ? 1 : depth); i++) {
    write("{}", m_format.indent);
  }
}

}  // namespace sdata
