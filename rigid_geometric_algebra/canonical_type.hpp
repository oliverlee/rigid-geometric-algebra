#pragma once

#include "rigid_geometric_algebra/blade.hpp"

#include <cstddef>

namespace rigid_geometric_algebra {

/// obtains the canonical type of a blade
/// @tparam T type
///
/// Defines member typedef `type` as the canonical blade type if `T` is a
/// specialization of `blade`. Otherwise; does not define member typedef `type`.
///
/// @{

template <class T>
struct canonical_type
{};

template <class A, std::size_t... Is>
struct canonical_type<blade<A, Is...>>
{
  using type = typename blade<A, Is...>::canonical_type;
};

template <class T>
using canonical_type_t = typename canonical_type<T>::type;

/// @}

}  // namespace rigid_geometric_algebra
