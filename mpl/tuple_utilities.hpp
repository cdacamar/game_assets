#pragma once

#include <tuple>

#include <mpl/list_operations.hpp>

namespace mpl {

namespace detail {

template <typename Func, typename Tuple, std::size_t... I>
void for_each_impl(Tuple&& t, Func&& f, std::index_sequence<I...>) {
  using swallow = int[];
  (void) swallow { (void(f(std::get<I>(t))), 0)... };
}

}
// find if a tuple contains T
template <typename T, typename Tuple>
struct tuple_contains;

template <typename T, typename... Ts>
struct tuple_contains<T, std::tuple<Ts...>> : list_contains<T, type_list<Ts...>> { };

// find if a tuple contains all unique Ts...
template <typename Tuple>
struct all_unique_tuple;

template <typename... Ts>
struct all_unique_tuple<std::tuple<Ts...>> : all_unique_types<type_list<Ts...>> { };

// for-each loop for tuple
template <typename Func, typename... Ts>
void for_each_tuple_element(std::tuple<Ts...>& t, Func&& f) {
  detail::for_each_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename Func, typename... Ts>
void for_each_tuple_element(const std::tuple<Ts...>& t, Func&& f) {
  detail::for_each_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(Ts)>{});
}

}
