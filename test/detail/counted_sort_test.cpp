#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/even.hpp"
#include "skytest/skytest.hpp"

#include <array>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::counted_sort;
  using ::rigid_geometric_algebra::detail::even;

  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  "num swaps"_test = [] {
    return expect(
        eq(true, even(counted_sort(std::array<int, 0>{}))) and
        eq(true, even(counted_sort(std::array{1}))) and
        eq(true, even(counted_sort(std::array{1, 2}))) and
        eq(false, even(counted_sort(std::array{2, 1}))) and
        eq(false, even(counted_sort(std::array{2, 1, 3}))) and
        eq(true, even(counted_sort(std::array{2, 3, 1}))) and
        eq(false, even(counted_sort(std::array{4, 1, 2, 3}))) and
        eq(true, even(counted_sort(std::array{1, 3, 4, 2}))) and
        eq(false, even(counted_sort(std::array{1, 4, 3, 2}))));
  };
}
