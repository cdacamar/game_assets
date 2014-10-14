#pragma once

#include <type_traits>

// test if a class has a member function with specified signature
#define GENERATE_HAS_MEMBER_FN(fn_name, signature) \
namespace mpl { \
  namespace detail { \
  struct has_member_fn_impl_ ## fn_name { \
    template <typename U, U> \
    struct chk { }; \
    \
    template <typename Class> \
    static std::true_type fn_name ## _(chk<signature, &Class::fn_name>*); \
    \
    template <typename U> \
    static std::false_type fn_name ## _(...); \
  }; \
  } \
  template <typename T> \
  struct has_member_fn_ ## fn_name : \
    decltype(detail::has_member_fn_impl_ ## fn_name::template fn_name ## _<T>(nullptr)) { }; \
}

// test if a class has a templated member function with a deduced argument
#define GENERATE_HAS_TEMPLATE_MEMBER_FN(fn_name, signature, ...) \
namespace mpl { \
  namespace detail { \
  struct has_template_member_fn_impl_ ## fn_name { \
    template <typename U, U> \
    struct chk { }; \
    \
    template <typename Class, typename Arg> \
    static std::true_type fn_name ## _(chk<signature(Arg)__VA_ARGS__, &Class::template fn_name<Arg>>*); \
    \
    template <typename U, typename Arg> \
    static std::false_type fn_name ## _(...); \
  }; \
  } \
  template <typename T, typename Arg> \
  struct has_template_member_fn_ ## fn_name : \
    decltype(detail::has_template_member_fn_impl_ ## fn_name::template fn_name ## _<T, Arg>(nullptr)) { }; \
}

// test if a class has a member function that takes a known argument
#define GENERATE_HAS_MEMBER_FN_ARG(fn_name, signature, ...) \
namespace mpl { \
  namespace detail { \
  struct has_member_fn_arg_impl_ ## fn_name { \
    template <typename U, U> \
    struct chk { }; \
    \
    template <typename Class, typename Arg> \
    static std::true_type fn_name ## _(chk<signature(Arg)__VA_ARGS__, &Class::fn_name>*); \
    \
    template <typename U, typename Arg> \
    static std::false_type fn_name ## _(...); \
  }; \
  } \
  template <typename T, typename Arg> \
  struct has_member_fn_arg_ ## fn_name : \
    decltype(detail::has_member_fn_arg_impl_ ## fn_name::template fn_name ## _<T, Arg>(nullptr)) { }; \
}
