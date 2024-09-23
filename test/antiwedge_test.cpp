#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <symengine/compat.hpp>

using ::rigid_geometric_algebra::antiwedge;
using ::rigid_geometric_algebra::left_complement;
using ::rigid_geometric_algebra::right_complement;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::param_ref;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
  using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;

  "antiwedge property"_ctest = [] {
    const auto a = G2::template blade<1>{3};
    const auto b = G2::template blade<0, 2>{4};

    return expect(eq(
        left_complement(right_complement(a) ^ right_complement(b)).coefficient,
        antiwedge(a, b).coefficient));
  };

  "antiwedge property"_ctest = [] {
    const auto a = G3::template blade<1>{3};
    const auto b = G3::template blade<0, 2>{4};
    const auto c = G3::template blade<1, 3>{2};
    const auto d = G3::template blade<0, 2, 3>{2};

    return expect(
        eq(left_complement(right_complement(a) ^ right_complement(b)),
           antiwedge(a, b)) and
        eq(left_complement(right_complement(c) ^ right_complement(b)),
           antiwedge(c, b)) and
        eq(left_complement(right_complement(c) ^ right_complement(a)),
           antiwedge(c, a)) and
        eq(left_complement(right_complement(d) ^ right_complement(a)),
           antiwedge(d, a)));
  };

  "antiwedge property (symengine)"_test = [] {
    const auto a = GS2::blade<1>{"a"};
    const auto b = GS2::blade<0, 2>{"b"};

    return expect(eq(
        left_complement(right_complement(a) ^ right_complement(b)),
        antiwedge(a, b)));
  };
}
