#pragma once

#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"

#include <complex>
#include <concepts>
#include <functional>

namespace rigid_geometric_algebra {

/// tag type denoting that the field identity value is unspecified
///
struct unspecified
{
  explicit unspecified() = default;
};

/// specifies the identity value for a type and a binary operation
/// @tparam T regular type
/// @tparam F closed binary operation
///
/// Defines the identity value for a type and closed binary operation. The
/// default value is `unspecified`.
///
/// A program may specialize `field_identity` for `T` and
/// `F = std::multiplies<>` if `T` is a program-defined type in order to
/// specify the value of "one<T>" (the multiplicative identity).
///
/// @see https://en.wikipedia.org/wiki/Field_(mathematics)
/// @see https://en.wikipedia.org/wiki/Monoid
///
template <std::regular T, class F>
inline constexpr auto field_identity = unspecified{};

/// field identity value for multiplication
///
template <std::regular T>
  requires std::floating_point<T> or
               detail::is_specialization_of_v<T, std::complex>
inline constexpr auto field_identity<T, std::multiplies<>> = T{1};

}  // namespace rigid_geometric_algebra
