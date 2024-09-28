#include "rigid_geometric_algebra/detail/indices_of.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"
#include "skytest/skytest.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::indices_of;
  using ::rigid_geometric_algebra::detail::structural_bitset;

  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;

  static constexpr auto equal = ::skytest::pred(std::ranges::equal);

  "with custom predicate"_test = [] {
    auto indices = std::array<std::size_t, 5>{};
    const auto last =
        indices_of(std::views::iota(0, 5), indices.begin(), [](auto value) {
          return value % 2 != 0;
        });

    return expect(
        equal(std::array{1, 3}, std::ranges::subrange(indices.begin(), last)));
  };

  "structural bitset indices"_test = [] {
    const auto bits = structural_bitset<4>{0b0101};

    auto indices = std::array<std::size_t, 4>{};
    const auto last = indices_of(bits, indices.begin());

    return expect(
        equal(std::array{0, 2}, std::ranges::subrange(indices.begin(), last)));
  };
}
