#pragma once

#include "rigid_geometric_algebra/detail/has_value.hpp"

#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

/// tuple element access
/// @tparam T type to access
/// @tparam V cv-ref qualified specialization of `multivector`
/// @param v `multivector`
///
/// Returns a reference to type `T` in `v`.
///
/// @{

namespace detail {

template <class B>
class get_fn
{
public:
  template <class V>
    requires detail::has_value_v<std::tuple_size<std::remove_cvref_t<V>>>
  constexpr static auto
  operator()(V&& v) noexcept -> decltype(std::forward<V>(v).template get<B>())
  {
    return std::forward<V>(v).template get<B>();
  }
};

}  // namespace detail

inline namespace cpo {

template <class B>
inline constexpr auto get = detail::get_fn<B>{};

}

/// @}

}  // namespace rigid_geometric_algebra
