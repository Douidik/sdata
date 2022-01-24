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
    auto *ptr = std::get_if<Traits<T>::index>(&variant);
    return ptr;
  }

  template<typename T, typename V>
  static auto &emplace_variant(V &variant, T data) {
    return variant.template emplace<Traits<T>::index>(data);
  }

public:
  using Native = std::variant<std::nullptr_t, Array, Sequence, float, int, bool, std::string>;

  Variant(auto data) {
    emplace_variant(m_variant, data);
  }

  Variant(std::initializer_list<class Node> sequence) {
    emplace_variant(m_variant, sequence);
  }

  Variant() : m_variant(nullptr) {}
  Variant(const Variant &) = default;

  inline Type type() const {
    return static_cast<Type>(m_variant.index());
  }

  template<typename T>
  inline bool is() const {
    return type() == Traits<T>::index;
  }

  inline auto &operator=(auto data) {
    return set(data);
  }

  const Native &native() const {
    return m_variant;
  }

  Native &native() {
    return m_variant;
  }

  template<typename T>
  inline auto &set(T data) {
    return emplace_variant(m_variant, data);
  }

  template<typename T>
  inline auto &as() {
    return type() != Traits<T>::index ? *get_variant<T>(m_variant)
                                      : emplace_variant<T>(m_variant, T {});
  }

  template<typename T>
  auto &get() {
    if (type() != Traits<T>::index) {
      throw_variant_unavailable<T>();
    }
    return *get_variant<T>(m_variant);
  }

  template<typename T>
  const auto &get() const {
    if (type() != Traits<T>::index) {
      throw_variant_unavailable<T>();
    }
    return *get_variant<T>(m_variant);
  }

  template<typename T>
  inline const auto *get_ptr() const {
    return get_variant<T>(m_variant);
  }

  template<typename T>
  inline auto *get_ptr() {
    return get_variant<T>(m_variant);
  }

  inline Variant &at(size_t n) {
    return get<Array>().at(n);
  }

  inline const Variant &at(size_t n) const {
    return get<Array>().at(n);
  }

  inline Variant &operator[](size_t n) {
    return at(n);
  }

  inline const Variant &operator[](size_t n) const {
    return at(n);
  }

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
