#pragma once

#include <tuple>
#include <type_traits>

#include <mpl/and_bools.hpp>

namespace mpl {

namespace detail {

struct null { };

template <typename Func, typename Tuple, std::size_t... I>
void for_each_impl(Tuple&& t, Func&& f, std::index_sequence<I...>) {
  using swallow = int[];
  (void) swallow { (void(f(std::get<I>(t))), 0)... };
}

}

template <typename T, typename Tuple>
struct tuple_contains;

template <typename T, typename... Ts>
struct tuple_contains<T, std::tuple<Ts...>> :
  std::conditional<
    std::is_same<
      std::tuple<typename std::conditional<std::is_same<T, Ts>::value, detail::null, Ts>::type...>,
      std::tuple<Ts...>
    >::value,
    std::false_type,
    std::true_type
  >::type
{ };

// find if a tuple contains all unique Ts...
template <typename Tuple>
struct all_unique_tuple;

template <typename... Ts>
struct all_unique_tuple<std::tuple<Ts...>> : std::true_type { };

template <typename T, typename... Ts>
struct all_unique_tuple<std::tuple<T, Ts...>> :
  std::conditional<
    and_bools<(!std::is_same<T, Ts>::value)...>::value,
    all_unique_tuple<std::tuple<Ts...>>,
    std::false_type
  >::type
{ };

template <typename Func, typename... Ts>
void for_each_tuple_element(std::tuple<Ts...>& t, Func&& f) {
  detail::for_each_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename Func, typename... Ts>
void for_each_tuple_element(const std::tuple<Ts...>& t, Func&& f) {
  detail::for_each_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(Ts)>{});
}

}
