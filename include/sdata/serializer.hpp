#ifndef SDATA_SERIALIZER_HPP
#define SDATA_SERIALIZER_HPP

#include <string_view>
#include <tuple>

namespace sdata {
class Node;

template<typename T>
struct Serializer : std::false_type {};

template<typename T>
struct SchemeProperty {
  std::string_view id;
  T &data;
};

template<typename S, typename... Ts>
struct Scheme : std::false_type {};

template<typename S, typename... Ts>
struct Scheme<S(Ts...)> : std::true_type {
  using Map = std::tuple<SchemeProperty<Ts &>...>;

  template<size_t I = 0>
  constexpr static void visit(auto &visitor, Map map) {
    if constexpr (I < sizeof...(Ts)) {
      visitor(std::get<I>(map));
      visit<I + 1>(visitor, map);
    }
  }

  virtual std::tuple<SchemeProperty<Ts &>...> map(S &schemed) = 0;
};

template<typename C>
struct Convert : std::true_type {
  virtual void encode(Node &, const C &) {}
  virtual void decode(const Node &, C &) {}
};

template<typename T>
concept is_serialized = Serializer<T>::value;

template<typename S>
concept is_schemed = is_serialized<S> && requires(Serializer<S> scheme, S schemed) {
  { scheme.map(schemed) } -> std::same_as<typename Serializer<S>::Map>;
};

template<typename C>
concept is_converted = is_serialized<C> && requires(Serializer<C> converter, Node node, C object) {
  {converter.encode(node, object)};
  {converter.decode(node, object)};
};

}  // namespace sdata

#endif
