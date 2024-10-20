#pragma once

#include "rigid_geometric_algebra/multivector_fwd.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {

/// determines if a type is a specialization of `multivector`
///
/// @{

template <class T>
struct is_multivector : std::false_type
{};

template <class A, auto... D>
struct is_multivector<multivector<A, D...>> : std::true_type
{};

template <class T>
inline constexpr auto is_multivector_v = is_multivector<T>::value;

/// @}

namespace detail {

/// implementation only concept
///
template <class T>
concept multivector = is_multivector_v<std::remove_cvref_t<T>>;

}  // namespace detail

}  // namespace rigid_geometric_algebra
