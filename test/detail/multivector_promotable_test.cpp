#include "rigid_geometric_algebra/algebra.hpp"
#include "rigid_geometric_algebra/detail/multivector_promotable.hpp"
#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::multivector_promotable;
  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;

  static_assert(multivector_promotable<G2::blade<>>);
  static_assert(multivector_promotable<G2::blade<0>>);
  static_assert(multivector_promotable<G2::blade<0, 1, 2>>);
  static_assert(multivector_promotable<G2::multivector<>>);
  static_assert(multivector_promotable<G2::multivector<{0, 1, 2}>>);

  static_assert(multivector_promotable<const G2::blade<>>);
  static_assert(multivector_promotable<G2::blade<>&>);
  static_assert(multivector_promotable<const G2::blade<>&>);
  static_assert(multivector_promotable<G2::blade<>&&>);
  static_assert(multivector_promotable<const G2::blade<>&&>);
  static_assert(multivector_promotable<const G2::multivector<>>);
  static_assert(multivector_promotable<G2::multivector<>&>);
  static_assert(multivector_promotable<const G2::multivector<>&>);
  static_assert(multivector_promotable<G2::multivector<>&&>);
  static_assert(multivector_promotable<const G2::multivector<>&&>);

  static_assert(not multivector_promotable<decltype(G2::zero)>);
}
