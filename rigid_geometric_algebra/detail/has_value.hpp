#pragma once

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// determines if a type has member typedef `value`
///
/// @{

template <class T, class = void>
struct has_value : std::false_type
{};

template <class T>
struct has_value<T, std::void_t<decltype(T::value)>> : std::true_type
{};

template <class T>
inline constexpr auto has_value_v = has_value<T>::value;
/// @}

}  // namespace rigid_geometric_algebra::detail
