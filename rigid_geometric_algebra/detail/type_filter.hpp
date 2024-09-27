#pragma once

#include "rigid_geometric_algebra/detail/type_concat.hpp"

#include <type_traits>
namespace rigid_geometric_algebra::detail {

/// filter types in a type list
///
/// Defines static member typedef `type` as a specialization of L with type
/// `T` in `Ts...` if `pred.operator()<T>()` is `true`.
///
/// @{

template <class L, auto pred>
struct type_filter
{};

template <class L, auto pred>
using type_filter_t = typename type_filter<L, pred>::type;

template <template <class...> class list, auto pred>
struct type_filter<list<>, pred>
{
  using type = list<>;
};

template <template <class...> class list, class T0, class... Ts, auto pred>
struct type_filter<list<T0, Ts...>, pred>
{
  using type = detail::type_concat_t<
      std::conditional_t<pred.template operator()<T0>(), list<T0>, list<>>,
      type_filter_t<list<Ts...>, pred>>;
};

/// @}

}  // namespace rigid_geometric_algebra::detail
