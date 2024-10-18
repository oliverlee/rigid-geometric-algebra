#pragma once

#include "rigid_geometric_algebra/field_identity.hpp"
#include "rigid_geometric_algebra/magma.hpp"

#include <concepts>
#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra {

/// specifies that a type represents a Field
/// @tparam T type
///
/// The field concept specifies that a type represents a Field. The type
// has closed binary operations called addition and multiplication. These
/// operations  satisfy the following properties:
/// * associativity of addition and multiplication
/// * commutativity of addition and multiplication
/// * additive and multiplicative identity
/// * additive inverses
/// * multiplicative inverses
/// * distributivity of multiplication over addition
///
/// This concept cannot check the associativity, commutativity, or
/// distributivity properties, nor the semantics of the inverse addition
/// (subtraction) and inverse multiplication (division) operation.
///
/// A program may specialize `field_identity` for `T` and
/// `F = std::multiplies<>` if `T` is a program-defined type in order to
/// specify the value of "one<T>" (the multiplicative identity).
///
/// Specializations with `F = std::plus<>` are ignored.
///
/// @see https://en.wikipedia.org/wiki/Field_(mathematics)
///
template <class T>
concept field =
    std::regular<T> and magma<T, std::plus<>> and magma<T, std::minus<>> and
    magma<T, std::multiplies<>> and magma<T, std::divides<>> and
    not std::same_as<
        unspecified,
        std::remove_cvref_t<decltype(field_identity<T, std::multiplies<>>)>>;

}  // namespace rigid_geometric_algebra
