#pragma once

#include <type_traits>

#include <mpl/and_bools.hpp>
#include <mpl/type_list.hpp>

namespace mpl {

namespace detail {

struct null { };

}

// find if a list contains a specific type
template <typename T, typename List>
struct list_contains;

template <typename T, typename... Ts>
struct list_contains<T, type_list<Ts...>> :
  std::conditional<
    std::is_same<
      type_list<typename std::conditional<std::is_same<T, Ts>::value, detail::null, Ts>::type...>,
      type_list<Ts...>
    >::value,
    std::false_type,
    std::true_type
  >::type
{ };

// find if a list contains all unique types
template <typename List>
struct all_unique_types;

template <typename... Ts>
struct all_unique_types<type_list<Ts...>> : std::true_type { };

template <typename T, typename... Ts>
struct all_unique_types<type_list<T, Ts...>> :
  std::conditional<
    and_bools<(!std::is_same<T, Ts>::value)...>::value,
    all_unique_types<type_list<Ts...>>,
    std::false_type
  >::type
{ };

}
