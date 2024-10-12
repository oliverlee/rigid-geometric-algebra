#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

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
  using ::rigid_geometric_algebra::blade_sum;

  "not invocable on non-blades"_test = [] {
    static_assert(not std::is_invocable_v<decltype(blade_sum), int>);
    static_assert(
        not std::is_invocable_v<decltype(blade_sum), decltype(G2::zero)>);
    static_assert(
        not std::is_invocable_v<decltype(blade_sum), G2::multivector<>>);

    return expect(true);
  };

  "not invocable on blade from different algebrass"_test = [] {
    static_assert(
        not std::
            is_invocable_v<decltype(blade_sum), G2::blade<>, G3::blade<1>>);

    return expect(true);
  };

  "no blades"_test = [] {
    static_assert(not std::is_invocable_v<decltype(blade_sum)>);

    return expect(true);
  };

  "single blade"_test = [] {
    const auto a = G2::blade<0>{42};
    const auto v = blade_sum(a);

    return expect(eq(a, get<G2::blade<0>>(v)) and eq(1, v.size()));
  };

  "multiple blades"_test = [] {
    const auto a = G2::blade<0>{3};
    const auto b = G2::blade<1, 2>{4};
    const auto v = blade_sum(a, b);

    return expect(eq(G2::multivector<{0}, {1, 2}>{3, 4}, v));
  };

  "multiple blades with same canonical form"_test = [] {
    const auto a = G2::blade<1, 2>{3};
    const auto b = G2::blade<2, 1>{4};
    const auto v = blade_sum(a, b);

    return expect(
        eq(G2::multivector<{1, 2}>{a.coefficient - b.coefficient}, v));
  };
}
