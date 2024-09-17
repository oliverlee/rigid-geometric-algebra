#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"

#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra {

namespace detail {

template <bool, class>
struct scalar_
{};

template <class A>
struct scalar_<true, A>
{
  using type = blade<A>;
};

template <bool, class>
struct antiscalar_
{};

template <class A>
struct antiscalar_<true, A>
{
  using type =
      decltype([]<std::size_t... Is>(
                   std::index_sequence<Is...>) -> blade<A, Is...> {
        return {};
      }(std::make_index_sequence<algebra_dimension_v<A>>{}));
};

}  // namespace detail

/// obtains the scalar type of an algebra
/// @tparam A algebra type
///
/// Defines member typedef `type` to be the `blade` specialization
/// corresponding to the scalar type of algebra `A`.
///
/// @{

template <class A>
struct scalar_type : detail::scalar_<is_algebra_v<A>, A>
{};

template <class A>
using scalar_type_t = typename scalar_type<A>::type;

/// @}

/// obtains the antiscalar type of an algebra
/// @tparam A algebra type
///
/// Defines member typedef `type` to be the `blade` specialization
/// corresponding to the antiscalar type of algebra `A`.
///
/// @{

template <class A>
struct antiscalar_type : detail::antiscalar_<is_algebra_v<A>, A>
{};

template <class A>
using antiscalar_type_t = typename antiscalar_type<A>::type;

/// @}

}  // namespace rigid_geometric_algebra
