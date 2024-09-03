#pragma once

#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

// TODO expose algebra_type
template <class T, class = void>
struct algebra_type
{};

template <class T>
struct algebra_type<T, std::void_t<typename T::algebra_type>>
    : std::type_identity<typename T::algebra_type>
{};

template <class Args, class = void>
struct common_algebra_type_
{};

template <class T0, class... Ts>
struct common_algebra_type_<
    type_list<T0, Ts...>,
    std::enable_if_t<(
        std::is_same_v<typename T0::algebra_type, typename Ts::algebra_type> and
        ...)>> : algebra_type<T0>
{};

}  // namespace detail

/// obtains the common algebra type
/// @tparam Ts types
///
/// Defines member typedef `type` as the algebra type if all `Ts` define member
/// typedef `algebra_type`. Otherwise; does not define member typedef `type`.
///
template <class... Ts>
struct common_algebra_type
    : detail::common_algebra_type_<detail::type_list<Ts...>>
{};

template <class... Ts>
using common_algebra_type_t = typename common_algebra_type<Ts...>::type;

}  // namespace rigid_geometric_algebra
