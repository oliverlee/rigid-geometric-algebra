#pragma once

namespace rigid_geometric_algebra::detail {

/// concatenate two type lists
///
/// Defines static member typedef `type` as a specialization of the common
/// *type-list*.
///
/// @{

template <class L1, class L2>
struct type_concat
{};

template <template <class...> class list, class... T1s, class... T2s>
struct type_concat<list<T1s...>, list<T2s...>>
{
  using type = list<T1s..., T2s...>;
};

template <class L1, class L2>
using type_concat_t = typename type_concat<L1, L2>::type;

/// @}

}  // namespace rigid_geometric_algebra::detail
