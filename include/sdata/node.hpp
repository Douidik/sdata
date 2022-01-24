#ifndef SDATA_NODE_HPP
#define SDATA_NODE_HPP

#include "misc/exception.hpp"
#include "serializer.hpp"
#include "token.hpp"
#include "variant.hpp"
#include <string_view>

namespace sdata {

class NodeException : public Exception {
public:
  NodeException(std::string_view description, const Node *node);

  inline const Node &node() const {
    return m_node;
  }

private:
  static std::string message(std::string_view description, const Node *node);
  const Node &m_node;
};

class Node : public Variant {
public:
  Node(std::string_view id) : m_id {parse_id(id)}, Variant() {}

  Node(std::string_view id, auto data) : m_id {parse_id(id)}, Variant(data) {}

  template<typename T>
  requires(is_serialized<T>) Node(std::string_view id, T serialized) :
    m_id {parse_id(id)},
    Variant(Sequence {}) {
    serialize<T>(serialized);
  }

  Node(Node &&) = default;
  Node(const Node &) = default;
  Node &operator=(const Node &) = default;
  using Variant::operator=;
  using Variant::operator[];

  inline std::string_view id() const {
    return m_id;
  }

  inline bool is_anonymous() const {
    return m_id.empty();
  }

  Node *search(std::string_view id);
  const Node *search(std::string_view id) const;
  Node &at(std::string_view id);
  const Node &at(std::string_view id) const;
  Node &operator[](std::string_view id);
  Node &insert(const Node &member);

  inline Node &insert(std::string_view id, auto data) {
    return insert(Node {id, data});
  }

  inline bool operator==(const Node &other) const {
    return id() == other.id() && Variant::operator==(other);
  }

  template<typename T>
  requires(is_serialized<T>) operator T() const {
    T object {};
    deserialize<T>(object);
    return object;
  }

private:
  template<typename S>
  requires(is_schemed<S>) void serialize(S &schemed) {
    auto visitor = [this](const auto &property) {
      insert(property.id, property.data);
    };

    Serializer<S>::visit(visitor, Serializer<S>().map(schemed));
  }

  template<typename S>
  requires(is_schemed<S>) void deserialize(S &schemed) const {
    auto visitor = [this]<typename T>(SchemeProperty<T> &property) {
      property.data = at(property.id).template get<T>();
    };

    Serializer<S>::visit(visitor, Serializer<S>().map(schemed));
  }

  template<typename C>
  requires(is_converted<C>) void serialize(const C &converted) {
    Serializer<C>().encode(*this, converted);
  }

  template<typename C>
  requires(is_converted<C>) void deserialize(C &converted) const {
    Serializer<C>().decode(*this, converted);
  }

  void throw_member_not_found(std::string_view id) const {
    throw NodeException {fmt("Member named '{}' not found in node sequence", id), this};
  }

  std::string parse_id(std::string_view id) const {
    if (!id.empty() && !Token::PATTERN.at(Token::ID).match(id)) {
      throw NodeException {"Naming convention violation [a-z A-Z 0-9 _]", this};
    }
    return std::string {id};
  }

  std::string m_id;
};

}  // namespace sdata

#endif
