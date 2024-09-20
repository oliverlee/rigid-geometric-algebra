#include "rigid_geometric_algebra/detail/swaps_to_sorted_dimensions.hpp"
#include "skytest/skytest.hpp"

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::swaps_to_sorted_dimensions;

  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  "num swaps"_test = [] {
    return expect(
        eq(0, swaps_to_sorted_dimensions()) and
        eq(0, swaps_to_sorted_dimensions(1UZ)) and
        eq(0, swaps_to_sorted_dimensions(1UZ, 2UZ)) and
        eq(1, swaps_to_sorted_dimensions(2UZ, 1UZ)) and
        eq(1, swaps_to_sorted_dimensions(2UZ, 1UZ, 3UZ)) and
        eq(2, swaps_to_sorted_dimensions(2UZ, 3UZ, 1UZ)) and
        eq(3, swaps_to_sorted_dimensions(4UZ, 1UZ, 2UZ, 3UZ)) and
        eq(2, swaps_to_sorted_dimensions(1UZ, 3UZ, 4UZ, 2UZ)) and
        eq(1, swaps_to_sorted_dimensions(1UZ, 4UZ, 3UZ, 2UZ)));
  };
}
