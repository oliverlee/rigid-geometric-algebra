#pragma once

#include "rigid_geometric_algebra/detail/tuple_cref.hpp"
#include "rigid_geometric_algebra/get.hpp"

#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

/// tuple element access
/// @tparam T type type to access
/// @tparam V tuple cv-ref qualified specialization of `std::tuple`
/// @tparam U default value type
/// @param v tuple reference
/// @param u default value
///
/// Returns get<B>(std::forward<V>(v))` or the default value
/// `std::forward<U>(u)`.
///
/// @{

namespace detail {

template <class B>
class get_or_fn
{
public:
  template <class V, class U>
    requires std::is_invocable_v<decltype(detail::tuple_cref), V&>
  constexpr static auto operator()(V&& v, U&& u) ->
      typename std::conditional_t<
          std::is_invocable_v<decltype(get<B>), V&&>,
          std::invoke_result<decltype(get<B>), V&&>,
          std::type_identity<U&&>>::type
  {
    if constexpr (std::is_invocable_v<decltype(get<B>), V>) {
      return get<B>(std::forward<V>(v));
    } else {
      return std::forward<U>(u);
    }
  }
};

}  // namespace detail

template <class B>
inline constexpr auto get_or = detail::get_or_fn<B>{};

/// @}

}  // namespace rigid_geometric_algebra
