#include "rigid_geometric_algebra/detail/are_dimensions_unique.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using namespace skytest::literals;
  using ::rigid_geometric_algebra::detail::are_dimensions_unique;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  static constexpr auto truthy = pred([](const auto& value) {
    return bool(value);
  });
  static constexpr auto falsy = pred([](const auto& value) {
    return not value;
  });

  "unique"_test = [] {
    return expect(
        truthy(are_dimensions_unique()) and
        truthy(are_dimensions_unique(1UZ)) and
        truthy(are_dimensions_unique(2UZ)) and
        truthy(are_dimensions_unique(1UZ, 2UZ)));
  };

  "not unique"_test = [] {
    return expect(
        falsy(are_dimensions_unique(1UZ, 2UZ, 1UZ)) and
        falsy(are_dimensions_unique(2UZ, 2UZ)));
  };
}
