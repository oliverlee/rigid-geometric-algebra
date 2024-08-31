#pragma once

#include "rigid_geometric_algebra/get.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

#include <concepts>
#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra {

/// multivector element access
/// @tparam B blade type to access
/// @tparam V multivector type
/// @tparam U default value type
/// @param v multivector
/// @param u default value
///
/// returns a copy or move initialized value of type `B` or the default value
/// `u`.
///
/// @note Requires:
/// * `U` is convertible to `B`
///
/// @{

namespace detail {

template <class B>
class get_or_fn
{
public:
  template <class V, std::convertible_to<B> U>
    requires is_multivector_v<std::remove_cvref_t<V>>
  constexpr static auto operator()(V&& v, U&& u) -> B
  {
    if constexpr (std::remove_cvref_t<V>::template contains<B>) {
      return get<B>(std::forward<V>(v));
    }

    return std::forward<U>(u);
  }
};

}  // namespace detail

template <class B>
inline constexpr auto get_or = detail::get_or_fn<B>{};

/// @}

}  // namespace rigid_geometric_algebra
