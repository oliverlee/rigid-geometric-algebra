#pragma once

#include "rigid_geometric_algebra/algebra_fwd.hpp"

#include <cstddef>

namespace rigid_geometric_algebra {

/// obtains the field type of an algebra
///
/// @{

template <class A>
struct algebra_field
{};

template <class F, std::size_t N>
struct algebra_field<algebra<F, N>>
{
  using type = F;
};

template <class A>
using algebra_field_t = typename algebra_field<A>::type;

/// @}

}  // namespace rigid_geometric_algebra
