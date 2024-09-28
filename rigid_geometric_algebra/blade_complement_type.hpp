#pragma once

#include "rigid_geometric_algebra/detail/disjoint_subset.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {
namespace detail {

template <bool, class>
struct blade_complement_type_
{};

template <
    template <class, std::size_t...> class blade_,
    class A,
    std::size_t... Is>
struct blade_complement_type_<true, blade_<A, Is...>>
{
  static constexpr auto complement_dimensions =
      []<std::size_t... Js>(std::index_sequence<Js...>) {
        return detail::disjoint_subset(Is..., Js...);
      }(std::make_index_sequence<algebra_dimension_v<A>>{});

  template <std::size_t... Ks>
  static constexpr auto type_impl(std::index_sequence<Ks...>)
      -> blade_<A, complement_dimensions[Ks]...>;

  using type = decltype(type_impl(
      std::make_index_sequence<complement_dimensions.size()>{}));
};

}  // namespace detail

/// obtain the blade complement type
/// @tparam T type
///
/// Defines member typedef `type as the complement type of `T` if `T` is a
/// `blade` type. Otherwise; does not define member typedef `type`.
///
/// The wedge product of a blade and the blade complement is equal to the unit
/// (hyper)volume of an algebra.
///
/// @see eq. 2.19
/// @see eq. 2.20
///
/// @{

template <class T>
struct blade_complement_type
    : detail::blade_complement_type_<detail::blade<T>, std::remove_cvref_t<T>>
{};

template <class T>
using blade_complement_type_t = typename blade_complement_type<T>::type;

/// @}

}  // namespace rigid_geometric_algebra
