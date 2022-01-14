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
  Node(std::string_view id, Native data) : Variant(data), m_id(id) {
    if (!is_anonymous() && !Token::PATTERN.at(Token::ID).match(id)) {
      throw NodeException {"Naming convention violated [a-z A-Z 0-9 _]", this};
    }
  }

  template<typename T>
  requires(Serializer<T>::value) explicit Node(const T &s) {
    Serializer<T>().encode(*this, s);
  }

  template<typename T>
  requires(Serializer<T>::value) operator T() const {
    T s {};
    Serializer<T>().decode(*this, s);
    return s;
  }

  Node(Node &&) = default;
  Node(const Node &) = default;
  Node &operator=(const Node &) = default;
  using Variant::operator=;

  inline std::string_view id() const {
    return m_id;
  }

  inline bool is_anonymous() const {
    return m_id.empty();
  }

  /// Emplaces a node to the sequence
  inline Node &emplace(auto &&...args) {
    return as<Sequence>().emplace_back(args...);
  }

  /// Access to a sequence element as a mutable-reference
  Node &operator[](std::string_view id);

  /// Access to a sequence element as a const-reference, throws a VariantException if the Sequence alternative is not available
  const Node &at(std::string_view id) const;

  inline bool compare(const Node &other) const {
    return id() == other.id() && Variant::compare(other);
  }

private:
  std::string m_id;
};

}  // namespace sdata

#endif
