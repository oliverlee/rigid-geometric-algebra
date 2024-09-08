#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class Args, class = void>
struct common_algebra_type_
{};

template <class T0, class... Ts>
struct common_algebra_type_<
    type_list<T0, Ts...>,
    std::enable_if_t<(
        std::is_same_v<algebra_type_t<T0>, algebra_type_t<Ts>> and ...)>>
    : algebra_type<T0>
{};

}  // namespace detail

/// obtains the common algebra type
/// @tparam Ts types
///
/// Defines member typedef `type` as the algebra type if all `Ts` define member
/// typedef `algebra_type`. Otherwise; does not define member typedef `type`.
///
/// @{

template <class... Ts>
struct common_algebra_type
    : detail::common_algebra_type_<detail::type_list<Ts...>>
{};

template <class... Ts>
using common_algebra_type_t = typename common_algebra_type<Ts...>::type;

/// @}

/// determines if a common algebra type exists
/// @tparam Ts types
///
/// @{

template <class... Ts>
struct has_common_algebra_type : detail::has_type<common_algebra_type<Ts...>>
{};

template <class... Ts>
inline constexpr auto has_common_algebra_type_v =
    has_common_algebra_type<Ts...>::value;

/// @}

}  // namespace rigid_geometric_algebra
