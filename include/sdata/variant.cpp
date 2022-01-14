#include "variant.hpp"
#include "misc/any_of.hpp"
#include "node.hpp"
#include <compare>

namespace sdata {

VariantException::VariantException(std::string_view description, const Variant *variant) :
  Exception(message(description, variant)),
  m_variant(*variant) {}

std::string VariantException::message(std::string_view description, const Variant *variant) {
  constexpr std::string_view PATTERN =
    "[sdata::VariantException raised]: {}\n"
    "with {{\n"
    "\tvariant of type <{}>\n"
    "}}";

  return fmt(PATTERN, description, variant->type());
}

bool Variant::compare(const Variant &other) const {
  constexpr auto visitor = []<typename T, typename U>(const T &a, const U &b) -> bool {
    if constexpr (!std::same_as<T, U>) {
      return false;
    }

    else if constexpr (any_of<T, Array, Sequence>) {
      return std::ranges::equal(a, b, [](const auto &m, const auto &n) { return m.compare(n); });
    }

    else if constexpr (any_of<T, float, int, bool, std::string>) {
      return a == b;
    }

    else {
      return true;
    }
  };

  return std::visit(visitor, native(), other.native());
}

}  // namespace sdata
