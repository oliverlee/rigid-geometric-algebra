#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

using rga = ::rigid_geometric_algebra::algebra<double, 2>;
using ::rigid_geometric_algebra::is_canonical_blade_order;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;

  "in order"_test = [] {
    return expect(
        is_canonical_blade_order<>() and
        is_canonical_blade_order<rga::blade<>>() and
        is_canonical_blade_order<
            rga::blade<>,
            rga::blade<0>,
            rga::blade<1>,
            rga::blade<2>,
            rga::blade<0, 1>,
            rga::blade<0, 2>,
            rga::blade<1, 2>,
            rga::blade<0, 1, 2>>());
  };

  "not in order"_test = [] {
    return expect(
        not is_canonical_blade_order<
            rga::blade<0>,
            rga::blade<>,
            rga::blade<1>,
            rga::blade<2>,
            rga::blade<0, 1>,
            rga::blade<0, 2>,
            rga::blade<1, 2>,
            rga::blade<0, 1, 2>>());
  };
}
