#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;
  using ::skytest::pred;

  static constexpr auto truthy = pred([](const auto& cond) {
    return bool(cond);
  });

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using ::rigid_geometric_algebra::is_canonical_blade_order;

  "in order"_test = [] {
    return expect(
        truthy(is_canonical_blade_order<>()) and
        truthy(is_canonical_blade_order<G2::blade<>>()) and
        truthy(is_canonical_blade_order<
               G2::blade<>,
               G2::blade<0>,
               G2::blade<1>,
               G2::blade<2>,
               G2::blade<0, 1>,
               G2::blade<0, 2>,
               G2::blade<1, 2>,
               G2::blade<0, 1, 2>>()));
  };

  "not in order"_test = [] {
    return expect(not truthy(
        is_canonical_blade_order<
            G2::blade<0>,
            G2::blade<>,
            G2::blade<1>,
            G2::blade<2>,
            G2::blade<0, 1>,
            G2::blade<0, 2>,
            G2::blade<1, 2>,
            G2::blade<0, 1, 2>>()));
  };

  "different algebra"_test = [] {
    return expect(not truthy(
        is_canonical_blade_order<G3::blade<1, 2>, G2::blade<0, 1, 2>>()));
  };

  "non-canonical blade"_test = [] {
    return expect(
        not truthy(is_canonical_blade_order<G2::blade<0, 2, 1>>()) and
        not truthy(
            is_canonical_blade_order<G2::blade<2, 1>, G2::blade<1, 0>>()));
  };

  "non-blade type"_test = [] {
    return expect(not truthy(is_canonical_blade_order<int>()));
  };
}
