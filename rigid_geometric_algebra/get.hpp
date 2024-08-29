#pragma once

#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/multivector.hpp"

#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra {

/// multivector element access
///
/// @{

namespace detail {

template <class B>
class get_fn
{
public:
  template <class Multivector, class D = std::remove_cvref_t<Multivector>>
    requires (is_specialization_of_v<D, multivector> and
              D::template contains<B>)
  constexpr static auto operator()(Multivector&& v) noexcept
      -> decltype(std::get<B>(std::forward<Multivector>(v)))
  {
    return std::get<B>(std::forward<Multivector>(v));
  }
};

}  // namespace detail

template <class B>
inline constexpr auto get = detail::get_fn<B>{};

/// @}

}  // namespace rigid_geometric_algebra
