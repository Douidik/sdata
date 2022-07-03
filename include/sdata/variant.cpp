#include "variant.hpp"
#include "misc/any_of.hpp"
#include "node.hpp"
#include "writer.hpp"

namespace sdata {

VariantException::VariantException(std::string_view description, const Variant *variant) :
  Exception(message(description, variant)),
  m_variant(*variant) {}

std::string VariantException::message(std::string_view description, const Variant *variant) {
  constexpr std::string_view PATTERN =
    "[sdata::VariantException raised]: {}\n"
    "with {{\n"
    "\tvariant<{}>: {}\n"
    "}}";

  return fmt(PATTERN, description, variant->type(), Writer(*variant, Format::inlined()).buffer());
}

bool Variant::operator==(const Variant &other) const {
  constexpr auto visitor = []<typename T, typename U>(const T &v, const U &w) {
    if constexpr (!std::same_as<T, U>) {
      return false;
    } else if constexpr (Traits<T>::index == Type::NIL) {
      return true;
    } else {
      return v == w;
    }
  };

  return std::visit(visitor, native(), other.native());
}

}  // namespace sdata
