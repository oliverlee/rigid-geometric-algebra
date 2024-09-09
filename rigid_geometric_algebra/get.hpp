#pragma once

#include "rigid_geometric_algebra/detail/tuple_cref.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra {

/// tuple element access
/// @tparam T type to access
/// @tparam V cv-ref qualified specialization of `std::tuple`
/// @param v tuple reference
///
/// Returns a reference to type `T` in `v`.
///
/// @{

namespace detail {

template <class B>
class get_fn
{

  template <class, class>
  struct tuple_element_count
  {};

  template <class T, class... Ts>
  struct tuple_element_count<T, std::tuple<Ts...>>
      : std::integral_constant<
            std::size_t,
            (static_cast<std::size_t>(std::is_same_v<T, Ts>) + ...)>
  {};

  template <class T, class Tuple>
  static constexpr auto tuple_element_count_v =
      tuple_element_count<T, Tuple>::value;

public:
  template <class V>
    requires std::is_invocable_v<decltype(detail::tuple_cref), V&&> and
                 (1UZ ==
                  tuple_element_count_v<
                      B,
                      std::remove_cvref_t<std::invoke_result_t<
                          decltype(detail::tuple_cref),
                          V &&>>>)
  constexpr static auto
  operator()(V&& v) noexcept -> decltype(std::get<B>(std::forward<V>(v)))
  {
    return std::get<B>(std::forward<V>(v));
  }
};

}  // namespace detail

template <class B>
inline constexpr auto get = detail::get_fn<B>{};

/// @}

}  // namespace rigid_geometric_algebra
