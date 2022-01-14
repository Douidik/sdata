#include "node.hpp"
#include <algorithm>

namespace sdata {

NodeException::NodeException(std::string_view description, const Node *node) :
  Exception(message(description, node)),
  m_node(*node) {}

std::string NodeException::message(std::string_view description, const Node *node) {
  constexpr std::string_view PATTERN =
    "[sdata::NodeException raised]: {}\n"
    "with {{\n"
    "\tnode '{}' of type <{}>\n"
    "}}";

  return fmt(PATTERN, description, node->id(), node->type());
}

Node &Node::operator[](std::string_view id) {
  Sequence &sequence = as<Sequence>();
  auto iter = std::ranges::find_if(sequence, [id](Node &n) { return n.id() == id; });

  if (iter != sequence.end()) {
    return *iter;
  } else {
    return sequence.emplace_back(id, nullptr);
  }
}

const Node &Node::at(std::string_view id) const {
  const Sequence &sequence = get<Sequence>();
  auto iter = std::ranges::find_if(sequence, [id](const Node &n) { return n.id() == id; });

  if (iter != sequence.end()) {
    return *iter;
  } else {
    throw NodeException {fmt("Member '{}' not found in sequence", id), this};
  }
}

}  // namespace sdata
