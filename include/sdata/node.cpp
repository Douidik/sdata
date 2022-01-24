#include "node.hpp"
#include "writer.hpp"
#include <algorithm>

namespace sdata {

NodeException::NodeException(std::string_view description, const Node *node) :
  Exception(message(description, node)),
  m_node(*node) {}

std::string NodeException::message(std::string_view description, const Node *node) {
  constexpr std::string_view PATTERN =
    "[sdata::NodeException raised]: {}\n"
    "with {{\n"
    "\tnode <{}> '{}': {}\n"
    "}}";

  return fmt(PATTERN, description, node->type(), node->id());
}

template<typename S>
auto *sequence_search(S &sequence, std::string_view id) {
  auto match_fn = [id](auto &node) -> bool {
    return node.id() == id;
  };

  auto iter = std::find_if(sequence.begin(), sequence.end(), match_fn);
  return iter != sequence.end() ? &*iter : nullptr;
}

Node *Node::search(std::string_view id) {
  return sequence_search(as<Sequence>(), id);
}

const Node *Node::search(std::string_view id) const {
  return sequence_search(get<Sequence>(), id);
}

Node &Node::at(std::string_view id) {
  Node *found = sequence_search(get<Sequence>(), id);

  if (!found) {
    throw_member_not_found(id);
  }

  return *found;
}

const Node &Node::at(std::string_view id) const {
  const Node *found = sequence_search(get<Sequence>(), id);

  if (!found) {
    throw_member_not_found(id);
  }

  return *found;
}

Node &Node::operator[](std::string_view id) {
  Node *found = search(id);
  return found ? *found : insert(id, nullptr);
}

Node &Node::insert(const Node &member) {
  Node *found = nullptr;  //= search(member.id());

  if (!found) {
    Sequence &sequence = std::get<Sequence>(m_variant);
    return get<Sequence>().emplace_back(member);
  } else {
    return *found = member;
  }
}

}  // namespace sdata
