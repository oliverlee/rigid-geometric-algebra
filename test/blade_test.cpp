#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <format>
#include <functional>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::function;
  using ::skytest::ne;
  using ::skytest::pred;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;

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

  "comparable with field type (if scalar)"_test = [] {
    return expect(eq(1., G2::blade<>{1}));
  };

  "not comparable with field type (if non-scalar)"_test = [] {
    const auto comparable = pred(
        function<"comparable">, []<class T1, class T2>(const T1&, const T2&) {
          return std::is_invocable_v<std::equal_to<>, T1, T2>;
        });

    return expect(
        not comparable(1., G2::blade<0>{}) and
        not comparable(1., G2::blade<0, 1, 2>{}));
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

  static constexpr auto equal =
      []<class T>(const std::type_identity_t<T>& lhs, const T& rhs) {
        return ::skytest::pred(std::equal_to<>{})(lhs, rhs);
      };

  "canonical form (G2)"_test = [] {
    return expect(
        equal({}, G2::blade<>::canonical_type::dimensions) and
        equal({0}, G2::blade<0>::canonical_type::dimensions) and
        equal({1}, G2::blade<1>::canonical_type::dimensions) and
        equal({2}, G2::blade<2>::canonical_type::dimensions) and
        equal({0, 1}, G2::blade<0, 1>::canonical_type::dimensions) and
        equal({0, 1}, G2::blade<1, 0>::canonical_type::dimensions) and
        equal({0, 2}, G2::blade<0, 2>::canonical_type::dimensions) and  // FIX
        equal({0, 2}, G2::blade<2, 0>::canonical_type::dimensions) and  // FIX
        equal({0, 1, 2}, G2::blade<0, 2, 1>::canonical_type::dimensions) and
        equal({0, 1, 2}, G2::blade<1, 2, 0>::canonical_type::dimensions) and
        equal({0, 1, 2}, G2::blade<0, 1, 2>::canonical_type::dimensions));
  };

  "canonical form (G3)"_test = [] {
    return expect(
        equal({}, G3::blade<>::canonical_type::dimensions) and
        equal({0}, G3::blade<0>::canonical_type::dimensions) and
        equal({1}, G3::blade<1>::canonical_type::dimensions) and
        equal({2}, G3::blade<2>::canonical_type::dimensions) and
        equal({3}, G3::blade<3>::canonical_type::dimensions) and
        equal({0, 1}, G3::blade<0, 1>::canonical_type::dimensions) and
        equal({0, 2}, G3::blade<0, 2>::canonical_type::dimensions) and
        equal({0, 3}, G3::blade<0, 3>::canonical_type::dimensions) and
        equal({1, 2}, G3::blade<1, 2>::canonical_type::dimensions) and
        equal({3, 1}, G3::blade<1, 3>::canonical_type::dimensions) and
        equal({2, 3}, G3::blade<2, 3>::canonical_type::dimensions) and
        equal({0, 1, 2}, G3::blade<0, 1, 2>::canonical_type::dimensions) and
        equal({0, 3, 1}, G3::blade<0, 1, 3>::canonical_type::dimensions) and
        equal({0, 2, 3}, G3::blade<0, 2, 3>::canonical_type::dimensions) and
        equal({3, 2, 1}, G3::blade<1, 2, 3>::canonical_type::dimensions) and
        equal({3, 2, 1}, G3::blade<3, 2, 1>::canonical_type::dimensions) and
        equal({0, 1, 2, 3}, G3::blade<0, 1, 2, 3>::canonical_type::dimensions));
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
