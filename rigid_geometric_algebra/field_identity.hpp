#pragma once

#include "rigid_geometric_algebra/magma.hpp"

#include <complex>
#include <concepts>
#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra {

/// specifies the identity value for a type and a binary operation
/// @tparam T regular type
/// @tparam F closed binary operation
///
/// Defines the identity value for a type and closed binary operation.
///
/// A program may specialize `field_identity` for `T` and
/// `F = std::multiplies<>` if `T` is a program-defined type in order to
/// specify the value of "one<T>" (the multiplicative identity).
///
/// @see https://en.wikipedia.org/wiki/Field_(mathematics)
/// @see https://en.wikipedia.org/wiki/Monoid
///
/// @{

template <std::regular T, class F>
  requires magma<T, F>
struct field_identity
{};

/// specialization for `std::floating_point` types and `std::multiplies<>`
///
template <std::floating_point T>
struct field_identity<T, std::multiplies<>> : std::integral_constant<T, T{1}>
{};

/// specialization for `std::complex` types and `std::multiplies<>`
///
template <std::floating_point T>
struct field_identity<std::complex<T>, std::multiplies<>>
    : std::integral_constant<T, T{1}>
{};

/// @}

/// reference to the field identity value
///
template <std::regular T, class F>
  requires magma<T, F>
inline constexpr const auto& field_identity_v = field_identity<T, F>::value;

}  // namespace rigid_geometric_algebra
