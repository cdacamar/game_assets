#pragma once

#include <type_traits>

// test if a class has a member function with specified signature
#define GENERATE_HAS_MEMBER_FN(fn_name) \
namespace mpl { \
  namespace detail { \
  template <typename, typename, typename=void> \
  struct has_member_fn_impl_ ## fn_name : std::false_type { }; \
  template <typename T, typename R, typename... Args> \
  struct has_member_fn_impl_ ## fn_name<T, R(Args...), \
    typename std::enable_if< \
      std::is_same< \
        decltype(std::declval<T>().fn_name(std::declval<Args>()...)), \
        R \
      >::value \
    >::type \
  > : std::true_type { }; \
  } \
  template <typename T, typename Signature> \
  using has_member_fn_ ## fn_name = \
    detail::has_member_fn_impl_ ## fn_name<T, Signature>; \
}
