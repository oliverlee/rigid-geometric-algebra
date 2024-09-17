#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <functional>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::field_identity;
  using ::rigid_geometric_algebra::detail::has_value_v;

  "field identity value"_test = [] {
    return expect(
        eq(float{1}, field_identity<float, std::multiplies<>>::value) and
        eq(double{1}, field_identity<double, std::multiplies<>>::value));
  };

  "no field identity value"_test = [] {
    return expect(not has_value_v<field_identity<int, std::multiplies<>>>);
  };
}
