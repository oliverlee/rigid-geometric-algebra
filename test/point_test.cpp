#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <format>
#include <symengine/expression.h>
#include <symengine/format.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;
  using ::rigid_geometric_algebra::multivector;

  "default constructible"_test = [] {
    return expect(eq(
        GS2::blade<0>{} + GS2::blade<1>{} + GS2::blade<2>{},
        GS2::point{}.multivector()));
  };

  "constructor requries all coefficients"_test = [] {
    return expect(
        std::is_constructible_v<GS2::point, int, int, int> and
        (not std::is_constructible_v<GS2::point, int, int>) and
        (not std::is_constructible_v<GS2::point, int>));
  };

  "copyable and comparable"_test = [] {
    const auto p = GS2::point{1, 2, 0};
    const auto q = p;  // NOLINT(performance-unnecessary-copy-initialization)

    return expect(eq(p, q));
  };

  "memnber ref qual"_test = [] {
    using P = GS2::point;
    using V = P::multivector_type;
    using S = typename P::value_type;

    // multivector
    static_assert(
        std::is_same_v<V&, decltype(std::declval<P&>().multivector())>);
    static_assert(
        std::is_same_v<
            const V&,
            decltype(std::declval<const P&>().multivector())>);
    static_assert(
        std::is_same_v<V&&, decltype(std::declval<P&&>().multivector())>);
    static_assert(
        std::is_same_v<
            const V&&,
            decltype(std::declval<const P&&>().multivector())>);

    // index
    static_assert(std::is_same_v<S&, decltype(std::declval<P&>()[0])>);
    static_assert(
        std::is_same_v<const S&, decltype(std::declval<const P&>()[0])>);
    static_assert(std::is_same_v<S&&, decltype(std::declval<P&&>()[1])>);
    static_assert(
        std::is_same_v<const S&&, decltype(std::declval<const P&&>()[0])>);

    return expect(true);
  };

  "indexable"_test = [] {
    const auto p = GS2::point{1, 2, 0};

    return expect(
        eq(::SymEngine::Expression{1}, p[0]) and
        eq(::SymEngine::Expression{2}, p[1]) and
        eq(::SymEngine::Expression{0}, p[2]));
  };

  "wedge"_test = [] {
    using S = ::SymEngine::Expression;
    using GS3 = ::rigid_geometric_algebra::algebra<S, 3>;
    const auto p = GS3::point{"pw", "px", "py", "pz"};
    const auto q = GS3::point{"qw", "qx", "qy", "qz"};

    const auto l =
        GS3::blade<0, 1>{S{"qx"} * S{"pw"} - S{"px"} * S{"qw"}} +
        GS3::blade<0, 2>{S{"qy"} * S{"pw"} - S{"py"} * S{"qw"}} +
        GS3::blade<0, 3>{S{"qz"} * S{"pw"} - S{"pz"} * S{"qw"}} +
        GS3::blade<2, 3>{S{"py"} * S{"qz"} - S{"pz"} * S{"qy"}} +
        GS3::blade<3, 1>{S{"pz"} * S{"qx"} - S{"px"} * S{"qz"}} +
        GS3::blade<1, 2>{S{"px"} * S{"qy"} - S{"py"} * S{"qx"}};

    const auto compare_each_element =
        []<class... Ts, class... Us>(
            const std::tuple<Ts...>& tup1,
            const std::tuple<Us...>& tup2,
            auto cmp) {
          return (cmp(std::get<Ts>(tup1), std::get<Us>(tup2)) and ...);
        };

    const auto cmp = [](const auto& b1, const auto& b2) {
      return eq(b1, b2) or eq(expand(b1.coefficient), expand(b2.coefficient));
    };

    return expect(compare_each_element(l, p ^ q, cmp));
  };

  "formattable"_test = [] {
    const auto p = GS2::point{"a", "b", "c"};

    return expect(eq(std::format("{}", p.multivector()), std::format("{}", p)));
  };
}
