#include "rigid_geometric_algebra/detail/disjoint_subset.hpp"
#include "skytest/skytest.hpp"

#include <algorithm>
#include <array>
#include <ranges>

auto main() -> int
{
  using namespace skytest::literals;
  using ::rigid_geometric_algebra::detail::disjoint_subset;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  static constexpr auto equal = pred(std::ranges::equal);

  "disjoint subset"_test = [] {
    return expect(
        equal(std::views::empty<int>, disjoint_subset()) and
        equal(std::array{1}, disjoint_subset(1)) and
        equal(std::array{1, 2, 3}, disjoint_subset(1, 2, 3)) and
        equal(std::array{1, 2}, disjoint_subset(1, 2, 3, 3)) and
        equal(std::array{2}, disjoint_subset(1, 2, 1, 3, 3)) and
        equal(std::array<int, 0>{}, disjoint_subset(3, 2, 1, 3, 2, 1)));
  };
}
