#pragma once

namespace rigid_geometric_algebra::detail {

/// insert types into a type list
///
/// Defines static member typedef `type` as a specialization of L with `Ts...`
/// inserted at the end.
///
/// @{

template <class L, class... Ts>
struct type_insert
{};

template <template <class...> class list, class... T1s, class... T2s>
struct type_insert<list<T1s...>, T2s...>
{
  using type = list<T1s..., T2s...>;
};

template <class L, class... Ts>
using type_insert_t = typename type_insert<L, Ts...>::type;

/// @}

}  // namespace rigid_geometric_algebra::detail
