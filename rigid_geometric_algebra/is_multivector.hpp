#pragma once

#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/multivector.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {

/// determines if a type is a specialization of `multivector`
///
/// @{

template <class T>
struct is_multivector : detail::is_specialization_of<T, multivector>
{};

template <class T>
inline constexpr auto is_multivector_v = is_multivector<T>::value;

/// @}

}  // namespace rigid_geometric_algebra
