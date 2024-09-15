#pragma once

#include "rigid_geometric_algebra/algebra_fwd.hpp"

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {

/// obtains the dimension of a geometric algebra
///
/// @{

template <class A>
struct algebra_dimension
{};

template <class F, std::size_t N>
struct algebra_dimension<algebra<F, N>>
    : std::integral_constant<std::size_t, N + 1>
{};

template <class A>
inline constexpr auto algebra_dimension_v = algebra_dimension<A>::value;

/// @}

}  // namespace rigid_geometric_algebra
