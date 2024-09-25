#pragma once

#include "rigid_geometric_algebra/detail/type_insert.hpp"

namespace rigid_geometric_algebra::detail {

/// type used as a list of other types
///
template <class... Ts>
struct type_list
{
  using type = type_list;

  /// insert `Ts...` at the end of into type list `L`
  ///
  /// `L` must be a type-list that defines static member typedef `type`
  ///
  template <class L>
  using insert_into_t = typename detail::type_insert_t<L, Ts...>::type;
};

}  // namespace rigid_geometric_algebra::detail
