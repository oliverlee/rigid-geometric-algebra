#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <format>
#include <functional>
#include <type_traits>

template <class T, class U>
static constexpr auto constructible =
    ::skytest::pred(std::is_constructible<T, U>{});

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::multivector;

  "constructible from blade addition"_test = [] {
    const auto a = G2::blade<1>{1};
    const auto b = G2::blade<2>{2};

    return expect(eq(multivector{a, b}, a + b));
  };

  "uses canonical blade form"_test = [] {
    const auto a = G2::blade<1>{1};
    const auto b = G2::blade<2>{2};
    const auto c = G2::blade<1, 2>{3};
    const auto d = G2::blade<2, 1>{3};

    return expect(
        eq(multivector{a, b}, a + b) and eq(multivector{a, b}, b + a) and
        eq(multivector{a, c}, a + c) and eq(multivector{a, c}, c + a) and
        eq(multivector{a, -c}, a + d) and eq(multivector{a, -c}, d + a));
  };

  "elements gettable"_test = [] {
    const auto a = G2::blade<1>{1};
    const auto v = multivector{a};

    using ::rigid_geometric_algebra::get;

    return expect(
        eq(a, get<G2::blade<1>>(v)) and
        not std::is_invocable_v<decltype(get<G2::blade<2>>), decltype(v)>);
  };

  "get return type"_test = [] {
    using B = G2::blade<1>;
    using V = multivector<G2, B::dimensions>;

    using ::rigid_geometric_algebra::get;
    using F = decltype(get<B>);

    return expect(
        std::is_same_v<B&, std::invoke_result_t<F, V&>> and
        std::is_same_v<const B&, std::invoke_result_t<F, const V&>> and
        std::is_same_v<B&&, std::invoke_result_t<F, V&&>> and
        std::is_same_v<const B&&, std::invoke_result_t<F, const V&&>>);
  };

  "constructible from a narrower multivector"_test = [] {
    using B1 = G2::blade<1>;
    using B2 = G2::blade<2>;

    using V1 = multivector<G2, B1::dimensions>;
    using V2 = multivector<G2, B1::dimensions, B2::dimensions>;

    const auto v1 = V1{B1{1}};
    const auto v2 = V2{v1};

    using ::rigid_geometric_algebra::get;

    return expect(eq(get<B1>(v1), get<B1>(v2)) and eq(B2{}, get<B2>(v2)));
  };

  "not constructible from wider multivector"_test = [] {
    return expect(
        not constructible<G2::multivector<>, G2::multivector<{0}>>() and
        not constructible<
            G2::multivector<{0}, {1}>,
            G2::multivector<{0}, {2}>>());
  };

  "addition of multivectors with disjoint blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}>{1, 2},
        multivector<G2, {0}>{1} + multivector<G2, {1}>{2}));
  };

  "addition of multivectors with overlapping blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}, {2}>{1, 5, 4},
        multivector<G2, {0}, {1}>{1, 2} + multivector<G2, {1}, {2}>{3, 4}));
  };

  "addition of multivectors with same blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}, {2}>{5, 7, 9},
        multivector<G2, {0}, {1}, {2}>{1, 2, 3} +
            multivector<G2, {0}, {1}, {2}>{4, 5, 6}));
  };

  "subtraction of multivectors with disjoint blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}>{1, -2},
        multivector<G2, {0}>{1} - multivector<G2, {1}>{2}));
  };

  "subtraction of multivectors with overlapping blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}, {2}>{1, -1, -4},
        multivector<G2, {0}, {1}>{1, 2} - multivector<G2, {1}, {2}>{3, 4}));
  };

  "subtraction of multivectors with same blade types"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}, {2}>{-3, -3, -3},
        multivector<G2, {0}, {1}, {2}>{1, 2, 3} -
            multivector<G2, {0}, {1}, {2}>{4, 5, 6}));
  };

  "scalar multiplication of a multivector"_test = [] {
    return expect(eq(
        multivector<G2, {0}, {1}, {2}>{2, 4, 6},
        2 * multivector<G2, {0}, {1}, {2}>{1, 2, 3}));
  };

  "addition of multivector with blade"_test = [] {
    return expect(
        eq(multivector<G2, {0}, {1}>{1, 2},
           multivector<G2, {0}>{1} + G2::blade<1>{2}) and
        eq(multivector<G2, {0}, {1}>{1, 2},
           G2::blade<1>{2} + multivector<G2, {0}>{1}));
  };

  "multivector wedge product"_test = [] {
    const auto b = G2::blade<>{1};
    const auto b0 = G2::blade<0>{2};
    const auto b1 = G2::blade<1>{3};
    const auto b2 = G2::blade<2>{4};

    return expect(eq(
        (b ^ b) + (b ^ b1) + (b ^ b2)           //
            + (b0 ^ b) + (b0 ^ b1) + (b0 ^ b2)  //
            + (b1 ^ b) + (b1 ^ b1) + (b1 ^ b2),
        multivector{b + b0 + b1} ^ multivector{b + b1 + b2}));
  };

  "multivector wedge product promotes blade"_test = [] {
    const auto b = G2::blade<>{1};
    const auto b0 = G2::blade<0>{2};
    const auto b1 = G2::blade<1>{3};
    const auto b2 = G2::blade<2>{4};

    return expect(
        eq((b ^ b2) + (b0 ^ b2) + (b1 ^ b2), multivector{b + b0 + b1} ^ b2) and
        eq((b2 ^ b) + (b2 ^ b0) + (b2 ^ b1), b2 ^ multivector{b + b0 + b1}));
  };

  "multivector wedge product can return zero"_test = [] {
    const auto b012 = G2::blade<0, 1, 2>{4};

    return expect(
        std::is_same_v<
            ::rigid_geometric_algebra::zero_constant<G2>,
            decltype(b012 ^ b012)>);
  };

  "comparison of different multivector types not hard error"_test = [] {
    using V1 = G2::multivector<>;
    using V2 = G2::multivector<{}, {1}>;

    return expect(not std::is_invocable_v<std::equal_to<>, V1, V2>);
  };

  "multivector types usable with structured bindings"_test = [] {
    const auto v = G2::multivector<{}, {1}>{0, 1};
    const auto [a, b] = v;

    return expect(eq(get<0>(v), a) and eq(get<1>(v), b));
  };

  "formattable"_test = [] {
    return expect(
        eq("0", std::format("{}", multivector<G2>{})) and
        eq("0.3", std::format("{}", multivector{G2::blade<>{0.3}})) and
        eq("0e₀", std::format("{}", multivector{G2::blade<0>{0}})) and
        eq("-1e₁ + -2e₀₂",
           std::format(
               "{}", multivector{G2::blade<1>{-1}, G2::blade<0, 2>{-2}})) and
        eq("0.3 + 2e₁ + 2e₀₁₂",
           std::format(
               "{}",
               multivector{
                   G2::blade<>{0.3}, G2::blade<1>{2}, G2::blade<0, 1, 2>{2}})));
  };
}
