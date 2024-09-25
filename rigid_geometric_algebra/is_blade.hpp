#pragma once

#include "rigid_geometric_algebra/blade.hpp"

#include <cstddef>
#include <type_traits>

namespace rigid_geometric_algebra {

/// determines if a type is a specialization of `blade`
///
/// @{

template <class T>
struct is_blade : std::false_type
{};

template <class A, std::size_t... Is>
struct is_blade<blade<A, Is...>> : std::true_type
{};

template <class T>
inline constexpr auto is_blade_v = is_blade<T>::value;

/// @}

namespace detail {

/// implementation only concept
///
template <class T>
concept blade = is_blade_v<std::remove_cvref_t<T>>;

}  // namespace detail

}  // namespace rigid_geometric_algebra
