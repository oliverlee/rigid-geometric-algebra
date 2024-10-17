#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include "test/skytest_ext.hpp"

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
  using ::skytest::equal_elements;
  using ::skytest::equal_ranges;
  using ::skytest::expect;

  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using GS3 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 3>;
  using ::rigid_geometric_algebra::multivector;

  "default constructible"_test = [] {
    return expect(
        eq(GS3::blade<0, 2, 3>{} + GS3::blade<0, 3, 1>{} +
               GS3::blade<0, 1, 2>{} + GS3::blade<3, 2, 1>{},
           GS3::plane{}.multivector()) and
        equal_ranges(G3::plane{}, std::array<double, 4>{}));
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

  "wedge of point and line"_test = [] {
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

    return expect(equal_elements(g, l ^ p));
  };

  "join of 3 points is a plane"_test = [] {
    using ::rigid_geometric_algebra::join;
    using ::rigid_geometric_algebra::right_complement;
    using ::rigid_geometric_algebra::get;

    const auto p = GS3::point{"1", "px", "py", "pz"};
    const auto q = GS3::point{"1", "qx", "qy", "qz"};
    const auto r = GS3::point{"1", "rx", "ry", "rz"};

    // Addition is not defined for geometric types (lines), so we add the
    // multivector components. The text for eq. 2.40 specifies that the wedge
    // products occur in three dimensions. We handle this by ignoring elements
    // containing the 0 basis dimension for n.
    const auto n =
        (p ^ q).multivector() + (q ^ r).multivector() + (r ^ p).multivector();

    // sign negation unnecessary since the complement is taken in dimension 4
    // instead of dimension 3.
    // see Table 2.5 (e321) and Table 2.28 (antiscalar)
    const auto d = right_complement((p ^ q ^ r).multivector());

    // see eq. 2.40
    const auto g = GS3::plane{
        get<GS3::blade<2, 3>>(n).coefficient,
        get<GS3::blade<3, 1>>(n).coefficient,
        get<GS3::blade<1, 2>>(n).coefficient,
        get<GS3::blade<0>>(d).coefficient};

    return expect(equal_elements(g, join(p, q, r)));
  };

  "formattable"_test = [] {
    const auto g = GS3::plane{"a", "b", "c", "d"};

    return expect(eq(std::format("{}", g.multivector()), std::format("{}", g)));
  };
}
