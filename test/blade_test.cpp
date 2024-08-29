#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::ne;

  using rga = ::rigid_geometric_algebra::algebra<double, 2>;

  "blade constructible"_test = [] {
    const auto a = rga::blade<>{};
    const auto b = rga::blade<0>{0};
    const auto c = rga::blade<1>{1};
    const auto d = rga::blade<2>{2};

    return expect(
        eq(0, a.coefficient) and eq(0, b.coefficient) and
        eq(1, c.coefficient) and eq(2, d.coefficient));
  };

  "blade comparable"_test = [] {
    const auto a = rga::blade<>{};
    const auto b = rga::blade<>{};
    const auto c = rga::blade<>{1};

    return expect(eq(a, b) and ne(a, c) and ne(b, c));
  };

  "blade grade"_test = [] {
    return expect(
        eq(0, rga::blade<>::grade) and      //
        eq(1, rga::blade<0>::grade) and     //
        eq(1, rga::blade<1>::grade) and     //
        eq(1, rga::blade<2>::grade) and     //
        eq(2, rga::blade<0, 1>::grade) and  //
        eq(2, rga::blade<1, 2>::grade) and  //
        eq(2, rga::blade<0, 2>::grade) and  //
        eq(3, rga::blade<0, 1, 2>::grade));
  };
}
