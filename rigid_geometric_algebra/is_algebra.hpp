#pragma once

#include "rigid_geometric_algebra/algebra_fwd.hpp"

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {

/// determines if a type is a specialization of `algebra`
///
/// @{

template <class T>
struct is_algebra : std::false_type
{};

template <class F, std::size_t N>
struct is_algebra<algebra<F, N>> : std::true_type
{};

template <class T>
inline constexpr auto is_algebra_v = is_algebra<T>::value;

/// @}

}  // namespace rigid_geometric_algebra
