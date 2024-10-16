#pragma once

#include "rigid_geometric_algebra/detail/geometric_interface.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/point.hpp"
#include "rigid_geometric_algebra/wedge.hpp"

#include <cstddef>
#include <format>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class A>
  requires is_algebra_v<A>
using plane_multivector_type_t = std::invoke_result_t<
    decltype(wedge),
    std::invoke_result_t<
        decltype(wedge),
        typename point<A>::multivector_type,
        typename point<A>::multivector_type>,
    typename point<A>::multivector_type>;

}  // namespace detail

template <class A>
  requires is_algebra_v<A>
class plane
    : public detail::geometric_interface<detail::plane_multivector_type_t<A>>
{
  static_assert(algebra_dimension_v<A> > 3);

  using base_type =
      detail::geometric_interface<detail::plane_multivector_type_t<A>>;

public:
  /// algebra type
  ///
  using algebra_type = typename base_type::algebra_type;

  /// blade scalar type
  ///
  using value_type = typename base_type::value_type;

  /// multivector type
  ///
  using multivector_type = typename base_type::multivector_type;

  /// default geometric type constructors
  ///
  using base_type::base_type;

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const plane&, const plane&) -> bool = default;

  /// @}
};

}  // namespace rigid_geometric_algebra

template <class A, class Char>
struct ::std::formatter<::rigid_geometric_algebra::plane<A>, Char>
    : ::std::formatter<
          ::rigid_geometric_algebra::detail::geometric_interface<
              typename ::rigid_geometric_algebra::plane<A>::multivector_type>,
          Char>
{};

template <class A>
struct ::glz::meta<::rigid_geometric_algebra::plane<A>>
    : ::glz::meta<::rigid_geometric_algebra::detail::geometric_interface<
          typename ::rigid_geometric_algebra::plane<A>::multivector_type>>
{};
