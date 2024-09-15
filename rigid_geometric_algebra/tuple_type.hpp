#pragma once

#include "rigid_geometric_algebra/detail/tuple_cref.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class T, class = void>
struct tuple_type_
{};

template <class T>
struct tuple_type_<
    T,
    std::enable_if_t<std::is_invocable_v<decltype(detail::tuple_cref), T>>>
{
  using type =
      std::remove_cvref_t<decltype(detail::tuple_cref(std::declval<T>()))>;
};

}  // namespace detail

/// obtain the underlying tuple type
/// @tparam T
///
///@{

template <class T>
struct tuple_type : detail::tuple_type_<T&>
{};

template <class T>
using tuple_type_t = typename tuple_type<T>::type;

///@}

}  // namespace rigid_geometric_algebra
