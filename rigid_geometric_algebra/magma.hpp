#pragma once

#include <concepts>

namespace rigid_geometric_algebra {
namespace detail {

template <class R, class F, class... Ts>
concept regular_invocable_r = std::regular_invocable<F, Ts...> and requires {
  { std::invoke(std::declval<F>(), std::declval<Ts>()...) } -> std::same_as<R>;
};

template <class F, class T>
concept closed_binary_operation =
    regular_invocable_r<T, F, T&, T&> and
    regular_invocable_r<T, F, T&, const T&> and
    regular_invocable_r<T, F, T&, T&&> and
    regular_invocable_r<T, F, T&, const T&&> and
    regular_invocable_r<T, F, const T&, T&> and
    regular_invocable_r<T, F, const T&, const T&> and
    regular_invocable_r<T, F, const T&, T&&> and
    regular_invocable_r<T, F, const T&, const T&&> and
    regular_invocable_r<T, F, T&&, T&> and
    regular_invocable_r<T, F, T&&, const T&> and
    regular_invocable_r<T, F, T&&, T&&> and
    regular_invocable_r<T, F, T&&, const T&&> and
    regular_invocable_r<T, F, const T&&, T&> and
    regular_invocable_r<T, F, const T&&, const T&> and
    regular_invocable_r<T, F, const T&&, T&&> and
    regular_invocable_r<T, F, const T&&, const T&&>;
}  // namespace detail

/// specifies that a type and operation form a Magma
/// @tparam T type
/// @tparam F regular invocable binary operation
///
/// The magma concept specifies that a type `T` and a binary operation `F` are
/// closed.
///
/// @see https://en.wikipedia.org/wiki/Magma_(algebra)
///
template <class T, class F>
concept magma =
    detail::closed_binary_operation<F&, T> and
    detail::closed_binary_operation<const F&, T> and
    detail::closed_binary_operation<F&&, T> and
    detail::closed_binary_operation<const F&&, T>;

}  // namespace rigid_geometric_algebra
