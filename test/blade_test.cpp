#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::ne;

  using rga = ::rigid_geometric_algebra::algebra<double, 2>;

  "constructible"_test = [] {
    const auto a = rga::blade<>{};
    const auto b = rga::blade<0>{0};
    const auto c = rga::blade<1>{1};
    const auto d = rga::blade<2>{2};

    return expect(
        eq(0, a.coefficient) and eq(0, b.coefficient) and
        eq(1, c.coefficient) and eq(2, d.coefficient));
  };

  "comparable"_test = [] {
    const auto a = rga::blade<>{};
    const auto b = rga::blade<>{};
    const auto c = rga::blade<>{1};

    return expect(eq(a, b) and ne(a, c) and ne(b, c));
  };

  "grade"_test = [] {
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

  "negatable"_test = [] {
    return expect(eq(rga::blade<0, 1>{1}, -rga::blade<0, 1>{-1}));
  };

  "canonical form"_test = [] {
    return expect(
        eq(rga::blade<>{}, rga::blade<>{}.canonical()) and
        eq(rga::blade<0>{}, rga::blade<0>{}.canonical()) and
        eq(rga::blade<1>{}, rga::blade<1>{}.canonical()) and
        eq(rga::blade<2>{}, rga::blade<2>{}.canonical()) and
        eq(rga::blade<0, 1>{}, rga::blade<0, 1>{}.canonical()) and
        eq(rga::blade<0, 1>{}, rga::blade<1, 0>{}.canonical()) and
        eq(rga::blade<0, 2>{}, rga::blade<0, 2>{}.canonical()) and
        eq(rga::blade<0, 2>{}, rga::blade<2, 0>{}.canonical()) and
        eq(rga::blade<0, 1, 2>{}, rga::blade<0, 2, 1>{}.canonical()) and
        eq(rga::blade<0, 1, 2>{}, rga::blade<1, 2, 0>{}.canonical()) and
        eq(rga::blade<0, 1, 2>{}, rga::blade<0, 1, 2>{}.canonical()));
  };

  "sign conversion for canonical form"_test = [] {
    return expect(
        eq(rga::blade<0, 1>{1}, -rga::blade<1, 0>{1}) and
        eq(rga::blade<0, 1, 2>{1}, rga::blade<0, 1, 2>{1}) and
        eq(rga::blade<0, 1, 2>{1}, rga::blade<1, 2, 0>{1}) and
        eq(rga::blade<0, 1, 2>{1}, rga::blade<2, 0, 1>{1}) and
        eq(rga::blade<0, 1, 2>{1}, -rga::blade<0, 2, 1>{1}) and
        eq(rga::blade<0, 1, 2>{1}, -rga::blade<1, 0, 2>{1}) and
        eq(rga::blade<0, 1, 2>{1}, -rga::blade<2, 1, 0>{1}));
  };

  "sign conversion for canonical form 3D"_test = [] {
    using rga3 = ::rigid_geometric_algebra::algebra<double, 3>;

    return expect(
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<0, 1, 2, 3>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, -rga3::blade<0, 1, 3, 2>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, -rga3::blade<0, 2, 1, 3>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, -rga3::blade<1, 0, 2, 3>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<0, 3, 1, 2>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<2, 0, 1, 3>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<0, 2, 3, 1>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<1, 2, 0, 3>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<1, 0, 3, 2>{1}) and
        eq(rga3::blade<0, 1, 2, 3>{1}, rga3::blade<3, 2, 1, 0>{1}));
  };
}
