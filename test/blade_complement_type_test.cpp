#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

#include <cstddef>
#include <type_traits>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
using ::rigid_geometric_algebra::blade_complement_type;
using ::rigid_geometric_algebra::blade_complement_type_t;
using ::rigid_geometric_algebra::zero_constant;
using ::rigid_geometric_algebra::detail::has_type_v;

auto main() -> int
{

  static_assert(
      std::is_same_v<G2::blade<>, blade_complement_type_t<G2::blade<0, 1, 2>>>);
  static_assert(
      std::is_same_v<G2::blade<1>, blade_complement_type_t<G2::blade<0, 2>>>);
  static_assert(
      std::is_same_v<G2::blade<0, 1, 2>, blade_complement_type_t<G2::blade<>>>);

  static_assert(not has_type_v<blade_complement_type<G2>>);
  static_assert(not has_type_v<blade_complement_type<zero_constant<G2>>>);
}
