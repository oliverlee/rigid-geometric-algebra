#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade_type_from.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <bool, class>
struct blade_complement_type_
{};

template <class B>
struct blade_complement_type_<true, B>
{
  using A = algebra_type_t<B>;

  using type = blade_type_from_dimensions_t<A, [] {
    auto missing = std::array<std::size_t, algebra_dimension_v<A> - B::grade>{};

    auto sorted = B::dimensions;
    std::ranges::sort(sorted);

    std::ranges::set_difference(
        std::views::iota(0UZ, algebra_dimension_v<A>), sorted, missing.begin());

    return missing;
  }()>::canonical_type;
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
