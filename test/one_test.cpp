#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <complex>
#include <symengine/compat.hpp>

using ::rigid_geometric_algebra::one;

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "one (floating point)"_ctest = [] {
    using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
    return expect(eq(1., one<G2>));
  };

  "one (complex)"_ctest = [] {
    using GZ2 = ::rigid_geometric_algebra::algebra<std::complex<double>, 2>;
    return expect(eq(1., one<GZ2>));
  };

  "one (symengine expression)"_test = [] {
    using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;
    return expect(eq(::SymEngine::Expression{1}, one<GS2>));
  };
}
