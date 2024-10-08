#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::multivector;
  using ::rigid_geometric_algebra::to_multivector;

  "to_multivector(blade)"_test = [] {
    return expect(
        eq(G2::multivector<G2::blade<1, 2>>{1},
           to_multivector(G2::blade<1, 2>{1})) and
        eq(G2::multivector<G2::blade<1, 2>>{1},
           -to_multivector(G2::blade<2, 1>{1})));
  };
}
