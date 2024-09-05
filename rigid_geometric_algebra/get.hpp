#pragma once

#include "rigid_geometric_algebra/is_multivector.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {

/// multivector element access
/// @tparam B blade type to access
/// @tparam V multivector type
/// @param v multivector
///
/// returns a reference to blade `B` in `v`
///
/// @{

namespace detail {

template <class B>
class get_fn
{
public:
  template <class V, class D = std::remove_cvref_t<V>>
    requires (is_multivector_v<D> and D::template contains<B>)
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
