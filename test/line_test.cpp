#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include "test/skytest_ext.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <ranges>
#include <symengine/compat.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::equal_ranges;
  using ::skytest::expect;
  using ::skytest::pred;

  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using GS3 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 3>;
  using ::rigid_geometric_algebra::multivector;

  "default constructible"_test = [] {
    return expect(equal_ranges(std::array<double, 6>{}, G3::line{}));
  };

  "constructor requries all coefficients"_test = [] {
    return expect(
        std::is_constructible_v<GS3::line, int, int, int, int, int, int> and
        (not std::is_constructible_v<GS3::line, int, int, int>) and
        (not std::is_constructible_v<GS3::line, int>));
  };

  "floating type constant constructible from ints"_test = [] {
    return expect(
        eq(G3::line{1., 2., 3., 0., 0., 0.}, G3::line{1, 2, 3, 0, 0, 0}));
  };

  "copyable and comparable"_test = [] {
    const auto l = G3::line{1, 2, 0, 0, 0, 3};
    const auto k = l;  // NOLINT(performance-unnecessary-copy-initialization)

    return expect(eq(l, k));
  };

  "aborts if constructed with non-perpendicular direction and moment"_test =
      [] { return expect(aborts([] { G3::line{1, 1, 1, 1, 1, 1}; })); };

  "multivector not assignable"_test = [] {
    using L = G3::line;
    using multivector_type = typename L::multivector_type;

    return expect(
        pred(std::is_same<
             const multivector_type&,
             decltype(std::declval<L&>().multivector())>{})() and
        pred(std::is_same<
             const multivector_type&,
             decltype(std::declval<const L&>().multivector())>{})() and
        pred(std::is_same<
             const multivector_type&&,
             decltype(std::declval<L&&>().multivector())>{})() and
        pred(std::is_same<
             const multivector_type&&,
             decltype(std::declval<const L&&>().multivector())>{})());
  };

  "coefficients not assignable"_test *
      std::tuple{G3{}, GS3{}} = []<class A>(A) {
    auto l1 = typename A::line{};
    auto l2 = typename A::line{};
    const auto k1 = typename A::line{};
    const auto k2 = typename A::line{};

    // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
    constexpr auto assignable = []<class T>(T&&) {
      return pred(std::is_assignable<T&&, double>{});
    };

    return expect(
        (not assignable(l1[0])()) and             //
        (not assignable(k1[0])()) and             //
        (not assignable(std::move(l2)[0])()) and  //
        (not assignable(std::move(k2)[0])()));
  };

  "formattable"_test = [] {
    const auto l = GS3::line{"v1", "v2", "v3", "m1", "m2", "m3"};

    return expect(eq(std::format("{}", l.multivector()), std::format("{}", l)));
  };

  "join of two points is a line"_test = [] {
    const auto p = G3::point{1, 1, 1, 0};
    const auto q = G3::point{1, 1, -1, 0};

    const auto l = p ^ q;

    const auto scale = std::views::transform([alpha = l[1]](auto value) {
      return value / alpha;
    });

    return expect(equal_ranges(std::array{0, 1, 0, 0, 0, 1}, l | scale));
  };
}
