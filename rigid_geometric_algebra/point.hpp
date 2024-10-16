#pragma once

#include "rigid_geometric_algebra/detail/geometric_interface.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/multivector_fwd.hpp"

#include <cstddef>
#include <format>
#include <utility>

namespace rigid_geometric_algebra {
namespace detail {

template <class A>
  requires is_algebra_v<A>
using point_multivector_type_t =
    decltype([]<std::size_t... Is>(std::index_sequence<Is...>)
                 -> ::rigid_geometric_algebra::multivector<A, { Is }...> {
      return {};
    }(std::make_index_sequence<algebra_dimension_v<A>>{}));

}  // namespace detail

template <class A>
  requires is_algebra_v<A>
class point
    : public detail::geometric_interface<detail::point_multivector_type_t<A>>
{
  static_assert(algebra_dimension_v<A> > 1);

  using base_type =
      detail::geometric_interface<detail::point_multivector_type_t<A>>;

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

  friend auto operator==(const point&, const point&) -> bool = default;

  /// @}
};

}  // namespace rigid_geometric_algebra

template <class A, class Char>
struct ::std::formatter<::rigid_geometric_algebra::point<A>, Char>
    : ::std::formatter<
          ::rigid_geometric_algebra::detail::geometric_interface<
              typename ::rigid_geometric_algebra::point<A>::multivector_type>,
          Char>
{};

template <class A>
struct ::glz::meta<::rigid_geometric_algebra::point<A>>
    : ::glz::meta<::rigid_geometric_algebra::detail::geometric_interface<
          typename ::rigid_geometric_algebra::point<A>::multivector_type>>
{};
