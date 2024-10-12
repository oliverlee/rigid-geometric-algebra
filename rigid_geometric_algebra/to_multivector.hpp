#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/detail/priority.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/multivector_fwd.hpp"

#include <utility>

namespace rigid_geometric_algebra {
namespace detail::to_multivector_impl {

auto to_multivector() -> void;

class fn
{
  template <detail::blade B>
  static constexpr auto
  impl(detail::priority<1>, B&& b) -> ::rigid_geometric_algebra::
      multivector<algebra_type_t<B>, canonical_type_t<B>::dimensions>
  {
    return {std::forward<B>(b).canonical()};
  }
  template <class T>
  static constexpr auto impl(detail::priority<0>, T&& t)
      -> decltype(to_multivector(std::forward<T>(t)))
  {
    return to_multivector(std::forward<T>(t));
  }

public:
  template <class T>
  static constexpr auto
  operator()(T&& t) -> decltype(impl(priority<1>{}, std::forward<T>(t)))
  {
    return impl(detail::priority<1>{}, std::forward<T>(t));
  }
};

}  // namespace detail::to_multivector_impl

inline namespace cpo {
inline constexpr auto to_multivector = detail::to_multivector_impl::fn{};
}

/// obtain the type after from invoking `to_multivector` on
/// `std::declval<T>()`
///
template <class T>
using to_multivector_t = decltype(cpo::to_multivector(std::declval<T>()));

}  // namespace rigid_geometric_algebra
