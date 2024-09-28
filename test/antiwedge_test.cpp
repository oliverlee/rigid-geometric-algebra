#include "rigid_geometric_algebra/complement.hpp"
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

    return expect(
        eq(left_complement(right_complement(a) ^ right_complement(b))
               .coefficient,
           antiwedge(a, b).coefficient) and
        eq(right_complement(left_complement(a) ^ left_complement(b))
               .coefficient,
           antiwedge(a, b).coefficient));
  };

  "antiwedge returns common blade component"_ctest = [] {
    const auto a = G2::template blade<0, 2>{3};
    const auto b = G2::template blade<0, 1>{2};

    return expect(
        eq(left_complement(right_complement(a) ^ right_complement(b)),
           antiwedge(a, b)) and
        eq(right_complement(left_complement(a) ^ left_complement(b)),
           antiwedge(a, b)) and
        eq(G2::blade<0>{-6}, antiwedge(a, b)));
  };

  "antiwedge returns zero constant"_ctest = [] {
    const auto a = G2::template blade<1>{3};
    const auto b = G2::template blade<2>{4};

    return expect(eq(G2::zero, antiwedge(a, b)));
  };

  "antiwedge of zero constant"_ctest = [] {
    return expect(eq(G2::zero, antiwedge(G2::zero, G2::zero)));
  };

  "antiwedge multivector"_ctest = [] {
    const auto a = G2::template blade<0>{3};
    const auto b = G2::template blade<1>{4};
    const auto c = G2::template blade<2>{5};

    return expect(
        eq(antiwedge(a, b) + antiwedge(a, c), antiwedge(a, b + c)) and
        eq(antiwedge(a, c) + antiwedge(b, c), antiwedge(a + b, c)) and
        eq(antiwedge(a, b) + antiwedge(a, c) + antiwedge(b, b) +
               antiwedge(b, c),
           antiwedge(a + b, b + c)));
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

    return expect(
        eq(left_complement(right_complement(a) ^ right_complement(b)),
           antiwedge(a, b)) and
        eq(right_complement(left_complement(a) ^ left_complement(b)),
           antiwedge(a, b)));
  };
}
