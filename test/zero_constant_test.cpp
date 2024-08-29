#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using rga = ::rigid_geometric_algebra::algebra<double, 2>;

  "equality"_test = [] { return expect(eq(rga::zero, rga::zero)); };

  "wedge"_test = [] {
    const auto v = rga::blade<1>{1};

    return expect(
        eq(rga::zero, rga::zero ^ rga::zero) and
        eq(rga::zero, rga::zero ^ v) and eq(rga::zero, v ^ rga::zero));
  };
}
