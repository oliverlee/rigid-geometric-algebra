#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <symengine/compat.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using GS3 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 3>;
  using ::rigid_geometric_algebra::multivector;

  static constexpr auto equal = pred(std::ranges::equal);

  "default constructible"_test = [] {
    return expect(
        eq(GS3::blade<0, 2, 3>{} + GS3::blade<0, 3, 1>{} +
               GS3::blade<0, 1, 2>{} + GS3::blade<3, 2, 1>{},
           GS3::plane{}.multivector()) and
        equal(G3::plane{}, std::array<double, 4>{}));
  };

  "constructor requires all coefficients"_test = [] {
    return expect(
        std::is_constructible_v<GS3::plane, int, int, int, int> and
        (not std::is_constructible_v<GS3::plane, int, int, int>) and
        (not std::is_constructible_v<GS3::plane, int, int>) and
        (not std::is_constructible_v<GS3::plane, int>));
  };

  "floating type constant constructible from ints"_test = [] {
    return expect(eq(G3::plane{1., 2., 3., 0.}, G3::plane{1, 2, 3, 0}));
  };

  "copyable and comparable"_test = [] {
    const auto g = GS3::plane{1, 2, 3, 4};
    const auto h = g;  // NOLINT(performance-unnecessary-copy-initialization)

    return expect(eq(g, h));
  };

  "memnber ref qual"_test = [] {
    using T = GS3::plane;
    using V = T::multivector_type;
    using S = typename T::value_type;

    // multivector
    static_assert(
        std::is_same_v<V&, decltype(std::declval<T&>().multivector())>);
    static_assert(
        std::is_same_v<
            const V&,
            decltype(std::declval<const T&>().multivector())>);
    static_assert(
        std::is_same_v<V&&, decltype(std::declval<T&&>().multivector())>);
    static_assert(
        std::is_same_v<
            const V&&,
            decltype(std::declval<const T&&>().multivector())>);

    // index
    static_assert(std::is_same_v<S&, decltype(std::declval<T&>()[0])>);
    static_assert(
        std::is_same_v<const S&, decltype(std::declval<const T&>()[0])>);
    static_assert(std::is_same_v<S&&, decltype(std::declval<T&&>()[1])>);
    static_assert(
        std::is_same_v<const S&&, decltype(std::declval<const T&&>()[0])>);

    return expect(true);
  };

  "indexable"_test = [] {
    const auto g = GS3::plane{1, 2, 3, 4};

    return expect(
        eq(::SymEngine::Expression{1}, g[0]) and
        eq(::SymEngine::Expression{2}, g[1]) and
        eq(::SymEngine::Expression{3}, g[2]) and
        eq(::SymEngine::Expression{4}, g[3]));
  };

  "index precondition"_test = [] {
    static constexpr auto g = G3::plane{};

    return expect(aborts([] { std::ignore = g[4]; }));
  };

  "wedge of point and line (multivector representation)"_test = [] {
    const auto p = GS3::point{"1", "px", "py", "pz"};

    const auto l = GS3::line{
        "lvx",
        "lvy",
        "lvz",
        "lmx",
        "lmy",
        "lmz",
    };

    // see eq. 2.38
    const auto g = GS3::plane{
        "lvy*pz - lvz*py + lmx",
        "lvz*px - lvx*pz + lmy",
        "lvx*py - lvy*px + lmz",
        "-lmx*px - lmy*py - lmz*pz",
    };

    const auto compare_each_element =
        []<class V>(const V& v1, const auto& v2, auto cmp) {
          return [&v1, &v2, &cmp]<auto... Is>(std::index_sequence<Is...>) {
            using ::rigid_geometric_algebra::get;
            return (cmp(v1.template get<Is>(), v2.template get<Is>()) and ...);
          }(std::make_index_sequence<V::size>{});
        };

    const auto cmp = [](const auto& b1, const auto& b2) {
      return eq(b1, b2) or eq(expand(b1.coefficient), expand(b2.coefficient));
    };

    return expect(compare_each_element(
        g.multivector(), l.multivector() ^ p.multivector(), cmp));
  };

  "formattable"_test = [] {
    const auto g = GS3::plane{"a", "b", "c", "d"};

    return expect(eq(std::format("{}", g.multivector()), std::format("{}", g)));
  };
}
