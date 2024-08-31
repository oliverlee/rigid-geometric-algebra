#pragma once

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// determines if a type has member typedef `type`
///
/// @{

template <class T, class = void>
struct has_type : std::false_type
{};

template <class T>
struct has_type<T, std::void_t<typename T::type>> : std::true_type
{};

template <class T>
inline constexpr auto has_type_v = has_type<T>::value;
/// @}

}  // namespace rigid_geometric_algebra::detail
