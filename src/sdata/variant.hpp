#ifndef SDATA_VARIANT_HPP
#define SDATA_VARIANT_HPP

#include "misc/any_of.hpp"
#include "misc/exception.hpp"
#include "misc/fmt.hpp"
#include "type.hpp"
#include <algorithm>
#include <compare>
#include <string>
#include <variant>
#include <vector>

namespace sdata {

using Array = std::vector<class Variant>;
using Sequence = std::vector<class Node>;

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

// std::variant<> wrapper
class Variant {
  using Native = std::variant<std::nullptr_t, Array, Sequence, float, int, bool, std::string>;

public:
  Variant(Native data = nullptr) : m_variant(data) {}

  /// Current alternative type
  inline Type type() const {
    return static_cast<Type>(m_variant.index());
  }

  /// Get a mutable-reference alternative of <T> or sets the variant to a default-constructed <T> if not available
  template<typename T>
  inline T &as() {
    if (!std::holds_alternative<T>(m_variant)) {
      m_variant = T {};
    }
    return std::get<T>(m_variant);
  }

  /// Get a mutable-reference alternative of <T> or the default_data if not available
  template<typename T>
  inline T &get(T &default_data) {
    return std::holds_alternative<T>(m_variant) ? std::get<T>(m_variant) : default_data;
  }

  /// Get a const-refernce alternative of <T> or the default_data if not available
  template<typename T>
  inline const T &get(const T &default_data) const {
    return std::holds_alternative<T>(m_variant) ? std::get<T>(m_variant) : default_data;
  }

  /// Get a mutable-refernce alternative of <T>, throws a VariantException if the alternative is not available
  template<typename T>
  inline T &get() {
    if (!std::holds_alternative<T>(m_variant)) {
      throw VariantException {"Variant does not currently contain data of type T", this};
    }
    return std::get<T>(m_variant);
  }

  /// Get a const-refernce alternative of <T>, throws a VariantException if the alternative is not available
  template<typename T>
  inline const T &get() const {
    if (!std::holds_alternative<T>(m_variant)) {
      throw VariantException {"Variant does not currently contain data of type T", this};
    }
    return std::get<T>(m_variant);
  }

  /// Is the currently held alternative of type <T>
  template<typename T>
  inline bool is() const {
    return std::holds_alternative<T>(m_variant);
  }

  auto &operator=(Native data) {
    m_variant = data;
    return *this;
  }

  /// Access an array element as a mutable-reference
  inline Variant &operator[](size_t index) {
    return as<Array>().at(index);
  }

  /// Access an array element as a const-reference, throws a VariantException if the Array alternative is not available
  inline const Variant &at(size_t index) const {
    return get<Array>().at(index);
  }

  /// Pushes a variant to the array
  inline Variant &push(auto &&...args) {
    return as<Array>().emplace_back(args...);
  }

  /// Returns a const-reference to the underlying std::variant<>
  const Native &native() const {
    return m_variant;
  }

  /// Returns a mutable-reference to the underlying std::variant<>
  Native &native() {
    return m_variant;
  }

  bool compare(const Variant &other) const;

protected:
  Native m_variant;
};

}  // namespace sdata

#endif
