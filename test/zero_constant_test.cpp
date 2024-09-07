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

  "negation"_test = [] {
    return expect(eq(rga::zero, -rga::zero) and eq(rga::zero, -(-rga::zero)));
  };

  "addition/subtraction of zero and zero"_test = [] {
    return expect(
        eq(rga::zero, rga::zero + rga::zero) and
        eq(rga::zero, rga::zero - rga::zero));
  };

  "addition of zero and blade"_test = [] {
    const auto v = rga::blade<1>{1};
    return expect(eq(v, rga::zero + v) and eq(v, v + rga::zero));
  };

  "subtraction of zero and blade"_test = [] {
    const auto v = rga::blade<1>{1};
    return expect(eq(-v, rga::zero - v) and eq(v, v - rga::zero));
  };
}
