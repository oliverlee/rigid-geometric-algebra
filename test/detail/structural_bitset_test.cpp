#include "rigid_geometric_algebra/detail/structural_bitset.hpp"
#include "skytest/skytest.hpp"

#include <algorithm>
#include <array>
#include <tuple>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::structural_bitset;

  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;

  using B2 = structural_bitset<2>;

  "bits constructor pre"_test = [] {
    return expect(aborts([] { B2{0b1000}; }));
  };

  "set pre"_test = [] { return expect(aborts([] { B2{}.set(4); })); };

  "set"_test = [] {
    return expect(
        eq(B2{0b0000'0001}, B2{}.set(0)) and
        eq(B2{0b0000'0010}, B2{}.set(1)) and
        eq(B2{0b0000'0011}, B2{}.set(0).set(1)));
  };

  "size"_test = [] {
    return expect(eq(2, B2{}.size()) and eq(3, structural_bitset<3>{}.size()));
  };

  "set"_test = [] {
    return expect(
        eq(0, B2{}.count()) and eq(1, B2{}.set(0).count()) and
        eq(1, B2{}.set(1).count()) and eq(2, B2{}.set(0).set(1).count()));
  };

  "reset"_test = [] {
    return expect(
        eq(B2{0b10}, B2{0b11}.reset(0)) and eq(B2{0b01}, B2{0b11}.reset(1)) and
        eq(B2{0b00}, B2{0b11}.reset(0).reset(1)) and
        eq(B2{0b10}, B2{0b11}.reset(0).reset(0)) and
        eq(B2{0b01}, B2{0b11}.reset(1).reset(1)));
  };

  "test pre"_test = [] {
    return expect(aborts([] { std::ignore = B2{}.test(4); }));
  };

  "test"_test = [] {
    return expect(
        eq(false, B2{}.test(0)) and eq(false, B2{}.test(1)) and
        eq(true, B2{1}.test(0)) and eq(false, B2{1}.test(1)) and
        eq(false, B2{2}.test(0)) and eq(true, B2{2}.test(1)) and
        eq(true, B2{3}.test(0)) and eq(true, B2{3}.test(1)));
  };

  "bitwise OR"_test = [] {
    return expect(
        eq(B2{3}, B2{1} | B2{2}) and eq(B2{3}, B2{2} | B2{1}) and
        eq(B2{}, B2{} | B2{}) and eq(B2{2}, B2{2} | B2{}) and
        eq(B2{2}, B2{} | B2{2}));
  };

  "bitwise AND"_test = [] {
    return expect(
        eq(B2{1}, B2{1} & B2{3}) and eq(B2{0}, B2{2} & B2{1}) and
        eq(B2{}, B2{} & B2{}) and eq(B2{0}, B2{0} & B2{2}) and
        eq(B2{2}, B2{3} & B2{2}));
  };

  "to unsigned "_test = [] {
    return expect(
        eq(0, B2{}.to_unsigned()) and eq(1, B2{1}.to_unsigned()) and
        eq(2, B2{2}.to_unsigned()) and eq(3, B2{3}.to_unsigned()));
  };

  "as range"_test = [] {
    const auto equal = ::skytest::pred(std::ranges::equal);

    return expect(
        equal(B2{0b01}, B2{0b01}) and
        equal(std::array{false, false}, B2{0b00}) and
        equal(std::array{true, true}, B2{0b11}) and
        equal(std::array{true, false}, B2{0b01}) and
        equal(std::array{false, true}, B2{0b10}));
  };
}
