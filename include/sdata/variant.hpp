#ifndef SDATA_VARIANT_HPP
#define SDATA_VARIANT_HPP

#include "misc/any_of.hpp"
#include "misc/exception.hpp"
#include "misc/fmt.hpp"
#include "traits.hpp"
#include <algorithm>
#include <variant>
#include <vector>

namespace sdata {

struct Array : std::vector<class Variant> {
  using std::vector<class Variant>::vector;
};

struct Sequence : std::vector<class Node> {
  using std::vector<class Node>::vector;
};

class VariantException : public Exception {
public:
  VariantException(std::string_view description, const class Variant *variant);

  inline const Variant &variant() const {
    return m_variant;
  }

private:
  static std::string message(std::string_view description, const class Variant *variant);
  const Variant &m_variant;
};

// std::variant<...> wrapper
class Variant {
  template<typename T, typename V>
  static auto *get_variant(V &variant) {
    return std::get_if<Traits<T>::index>(&variant);
  }

  template<typename T, typename V>
  static auto &emplace_variant(V &variant, T data) {
    return variant.template emplace<Traits<T>::index>(data);
  }

public:
  /// Wrapped std::variant templated type
  using Native = std::variant<std::nullptr_t, Array, Sequence, float, int, bool, std::string>;

  /// Variant data constructor
  Variant(auto data) {
    emplace_variant(m_variant, data);
  }

  /// Variant sequence constructor
  Variant(std::initializer_list<class Node> sequence) {
    emplace_variant(m_variant, sequence);
  }

  Variant() : m_variant(nullptr) {}

  Variant(const Variant &) = default;

  /// Variant alternative index
  inline Type type() const {
    return static_cast<Type>(m_variant.index());
  }

  /// Does the variant contain value of type <T> ?
  template<typename T>
  inline bool is() const {
    return type() == Traits<T>::index;
  }

  /// Assigns the variant value
  inline auto &operator=(auto data) {
    return set(data);
  }

  /// Get the wrapped std::variant
  const Native &native() const {
    return m_variant;
  }

  /// Get the wrapped std::variant
  Native &native() {
    return m_variant;
  }

  /// Assigns the variant value
  template<typename T>
  inline auto &set(T data) {
    return emplace_variant(m_variant, data);
  }

  /// Get the variant alternative <T> or a default-constructed value if not available
  template<typename T>
  inline auto &as() {
    return is<T>() ? *get_variant<T>(m_variant) : emplace_variant<T>(m_variant, T {});
  }

  /// Get a variant alternative <T> reference
  template<typename T>
  auto &get() {
    if (type() != Traits<T>::index) {
      throw_variant_unavailable<T>();
    }
    return *get_variant<T>(m_variant);
  }

  /// Get a variant alternative <T> const-reference
  template<typename T>
  const auto &get() const {
    if (type() != Traits<T>::index) {
      throw_variant_unavailable<T>();
    }
    return *get_variant<T>(m_variant);
  }

  /// Get a variant alternative <T> pointer or nullptr if not available
  template<typename T>
  inline const auto *get_ptr() const {
    return get_variant<T>(m_variant);
  }

  /// Get a variant alternative <T> const-pointer or nullptr if not available
  template<typename T>
  inline auto *get_ptr() {
    return get_variant<T>(m_variant);
  }

  /// Get the n-th item of the array
  inline Variant &at(size_t n) {
    return get<Array>().at(n);
  }

  /// Get the n-th item of the array
  inline const Variant &at(size_t n) const {
    return get<Array>().at(n);
  }

  /// Get the n-th item of the array
  inline Variant &operator[](size_t n) {
    return at(n);
  }

  /// Get the n-th item of the array
  inline const Variant &operator[](size_t n) const {
    return at(n);
  }

  /// Recursive variant value comparator
  bool operator==(const Variant &other) const;

protected:
  template<typename T>
  void throw_variant_unavailable() const {
    throw VariantException {
      fmt("Variant alternative not available for type <{}>", static_cast<Type>(Traits<T>::index)),
      this,
    };
  }

  Native m_variant;
};

}  // namespace sdata

#endif
