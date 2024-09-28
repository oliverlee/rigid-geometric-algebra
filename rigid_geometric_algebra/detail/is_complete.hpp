#pragma once

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// determine if a type is complete at time of instantiation of this trait
///
/// @see
/// https://stackoverflow.com/questions/21119281/using-sfinae-to-check-if-the-type-is-complete-or-not
///

template <class T, class = void>
struct is_complete_ : std::false_type
{};

template <class T>
struct is_complete_<T, std::enable_if_t<sizeof(T) != 0>> : std::true_type
{};

template <class T>
struct is_complete : is_complete_<T>
{};

template <class T>
inline constexpr auto is_complete_v = is_complete<T>::value;

}  // namespace rigid_geometric_algebra::detail
