#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <functional>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::field_identity;

  "field identity"_test = [] {
    return expect(
        eq(float{1}, field_identity<float, std::multiplies<>>) and
        eq(double{1}, field_identity<double, std::multiplies<>>));
  };

  "no field identity value"_test = [] {
    return expect(
        std::is_same_v<
            rigid_geometric_algebra::unspecified,
            std::remove_cvref_t<
                decltype(field_identity<int, std::multiplies<>>)>>);
  };
}
