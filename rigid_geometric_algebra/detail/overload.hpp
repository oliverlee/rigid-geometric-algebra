#pragma once

#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"

namespace rigid_geometric_algebra::detail {

/// tag used to describe a synthesized overload set
/// @tparam Op operation tag
/// @tparam Args argument types
///
template <class Op, class... Args>
struct overload
{
  using operation_type = Op;
};

/// obtains the operation tag from a overload tag
/// @tparam Ov specialization of `overload`
///
template <class Ov>
  requires is_specialization_of_v<Ov, overload>
using operation_t = typename Ov::operation_type;

}  // namespace rigid_geometric_algebra::detail
