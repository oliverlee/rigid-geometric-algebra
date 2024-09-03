#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using rga = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::multivector;

  "constructible from blade addition"_test = [] {
    const auto a = rga::blade<1>{1};
    const auto b = rga::blade<2>{2};

    return expect(eq(multivector{a, b}, a + b));
  };

  "uses canonical blade form"_test = [] {
    const auto a = rga::blade<1>{1};
    const auto b = rga::blade<2>{2};
    const auto c = rga::blade<1, 2>{3};
    const auto d = rga::blade<2, 1>{3};

    return expect(
        eq(multivector{a, b}, a + b) and eq(multivector{a, b}, b + a) and
        eq(multivector{a, c}, a + c) and eq(multivector{a, c}, c + a) and
        eq(multivector{a, -c}, a + d) and eq(multivector{a, -c}, d + a));
  };

  "elements gettable"_test = [] {
    const auto a = rga::blade<1>{1};
    const auto v = multivector{a};

    using ::rigid_geometric_algebra::get;

    return expect(
        eq(a, get<rga::blade<1>>(v)) and
        not std::is_invocable_v<decltype(get<rga::blade<2>>), decltype(v)>);
  };

  "get return type"_test = [] {
    using B = rga::blade<1>;
    using V = multivector<rga, B>;

    using ::rigid_geometric_algebra::get;
    using F = decltype(get<B>);

    return expect(
        std::is_same_v<B&, std::invoke_result_t<F, V&>> and
        std::is_same_v<const B&, std::invoke_result_t<F, const V&>> and
        std::is_same_v<B&&, std::invoke_result_t<F, V&&>> and
        std::is_same_v<const B&&, std::invoke_result_t<F, const V&&>>);
  };

  "constructible from a narrower multivector"_test = [] {
    using B1 = rga::blade<1>;
    using B2 = rga::blade<2>;

    using V1 = multivector<rga, B1>;
    using V2 = multivector<rga, B1, B2>;

    const auto v1 = V1{B1{1}};
    const auto v2 = V2{v1};

    using ::rigid_geometric_algebra::get;

    return expect(eq(get<B1>(v1), get<B1>(v2)) and eq(B2{}, get<B2>(v2)));
  };
}
