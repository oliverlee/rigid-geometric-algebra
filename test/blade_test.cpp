#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <format>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::ne;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;

  "constructible"_test = [] {
    const auto a = G2::blade<>{};
    const auto b = G2::blade<0>{0};
    const auto c = G2::blade<1>{1};
    const auto d = G2::blade<2>{2};

    return expect(
        eq(0, a.coefficient) and eq(0, b.coefficient) and
        eq(1, c.coefficient) and eq(2, d.coefficient));
  };

  "comparable"_test = [] {
    const auto a = G2::blade<>{};
    const auto b = G2::blade<>{};
    const auto c = G2::blade<>{1};

    return expect(eq(a, b) and ne(a, c) and ne(b, c));
  };

  "grade"_test = [] {
    return expect(
        eq(0, G2::blade<>::grade) and      //
        eq(1, G2::blade<0>::grade) and     //
        eq(1, G2::blade<1>::grade) and     //
        eq(1, G2::blade<2>::grade) and     //
        eq(2, G2::blade<0, 1>::grade) and  //
        eq(2, G2::blade<1, 2>::grade) and  //
        eq(2, G2::blade<0, 2>::grade) and  //
        eq(3, G2::blade<0, 1, 2>::grade));
  };

  "negatable"_test = [] {
    return expect(eq(G2::blade<0, 1>{1}, -G2::blade<0, 1>{-1}));
  };

  "canonical form"_test = [] {
    return expect(
        eq(G2::blade<>{}, G2::blade<>{}.canonical()) and
        eq(G2::blade<0>{}, G2::blade<0>{}.canonical()) and
        eq(G2::blade<1>{}, G2::blade<1>{}.canonical()) and
        eq(G2::blade<2>{}, G2::blade<2>{}.canonical()) and
        eq(G2::blade<0, 1>{}, G2::blade<0, 1>{}.canonical()) and
        eq(G2::blade<0, 1>{}, G2::blade<1, 0>{}.canonical()) and
        eq(G2::blade<0, 2>{}, G2::blade<0, 2>{}.canonical()) and
        eq(G2::blade<0, 2>{}, G2::blade<2, 0>{}.canonical()) and
        eq(G2::blade<0, 1, 2>{}, G2::blade<0, 2, 1>{}.canonical()) and
        eq(G2::blade<0, 1, 2>{}, G2::blade<1, 2, 0>{}.canonical()) and
        eq(G2::blade<0, 1, 2>{}, G2::blade<0, 1, 2>{}.canonical()));
  };

  "sign conversion for canonical form"_test = [] {
    return expect(
        eq(G2::blade<0, 1>{1}, -G2::blade<1, 0>{1}) and
        eq(G2::blade<0, 1, 2>{1}, G2::blade<0, 1, 2>{1}) and
        eq(G2::blade<0, 1, 2>{1}, G2::blade<1, 2, 0>{1}) and
        eq(G2::blade<0, 1, 2>{1}, G2::blade<2, 0, 1>{1}) and
        eq(G2::blade<0, 1, 2>{1}, -G2::blade<0, 2, 1>{1}) and
        eq(G2::blade<0, 1, 2>{1}, -G2::blade<1, 0, 2>{1}) and
        eq(G2::blade<0, 1, 2>{1}, -G2::blade<2, 1, 0>{1}));
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

  "addition"_test = [] {
    const auto b0 = G2::blade<>{};
    const auto b1 = G2::blade<>{1};
    const auto b2 = G2::blade<>{2};

    return expect(
        eq(b0, b0 + b0) and eq(b1, b0 + b1) and eq(b2, b0 + b2) and
        eq(b1, b1 + b0) and eq(b2, b2 + b0) and eq(b2, b1 + b1) and
        eq(b2, b2 + b0 + b0));
  };

  "addition of blades with same canonical form"_test = [] {
    return expect(
        eq(G2::blade<1, 2>{-1}, G2::blade<1, 2>{1} + G2::blade<2, 1>{2}));
  };

  "subtraction"_test = [] {
    const auto b0 = G2::blade<>{};
    const auto b1 = G2::blade<>{1};
    const auto b2 = G2::blade<>{2};

    return expect(
        eq(b0, b0 - b0) and eq(-b1, b0 - b1) and eq(-b2, b0 - b2) and
        eq(b1, b1 - b0) and eq(b2, b2 - b0) and eq(b0, b1 - b1) and
        eq(b2, b2 - b0 - b0));
  };

  "subtraction of blades with same canonical form"_test = [] {
    return expect(
        eq(G2::blade<1, 2>{3}, G2::blade<1, 2>{1} - G2::blade<2, 1>{2}));
  };

  "scalar multiplication"_test = [] {
    return expect(
        eq(G2::blade<>{2}, 2 * G2::blade<>{1}) and
        eq(G2::blade<>{2}, 2.0 * G2::blade<>{1}));
  };

  "wedge product with unique dimensions"_test = [] {
    return expect(
        eq(G2::blade<>{1}, G2::blade<>{1} ^ G2::blade<>{1}) and
        eq(G2::blade<0>{1}, G2::blade<>{1} ^ G2::blade<0>{1}) and
        eq(G2::blade<0>{1}, G2::blade<0>{1} ^ G2::blade<>{1}) and
        eq(G2::blade<1>{1}, G2::blade<>{1} ^ G2::blade<1>{1}) and
        eq(G2::blade<2>{1}, G2::blade<>{1} ^ G2::blade<2>{1}) and
        eq(G2::blade<2, 1>{1}, G2::blade<2>{1} ^ G2::blade<1>{1}) and
        eq(G2::blade<1, 2>{1}, G2::blade<1>{1} ^ G2::blade<2>{1}));
  };

  "wedge product with repeated dimensions"_test = [] {
    return expect(
        eq(G2::zero, G2::blade<0>{1} ^ G2::blade<0>{1}) and
        eq(G2::zero, G2::blade<1>{1} ^ G2::blade<1>{1}) and
        eq(G2::zero, G2::blade<0, 1>{1} ^ G2::blade<1, 2>{1}));
  };

  "formattable"_test = [] {
    return expect(
        eq("0.3", std::format("{}", G2::blade<>{0.3})) and
        eq("0e₀", std::format("{}", G2::blade<0>{0})) and
        eq("2e₁", std::format("{}", G2::blade<1>{2})) and
        eq("2e₀₁₂", std::format("{}", G2::blade<0, 1, 2>{2})) and
        eq("2e₂₁", std::format("{}", G2::blade<2, 1>{2})));
  };
}
