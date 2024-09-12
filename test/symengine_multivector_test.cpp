#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <format>
#include <symengine/expression.h>
#include <symengine/format.hpp>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using GS2 = ::rigid_geometric_algebra::algebra<::SymEngine::Expression, 2>;
  using ::rigid_geometric_algebra::multivector;

  "constructible from blade addition"_test = [] {
    const auto a = GS2::blade<1>{::SymEngine::Expression{"a"}};
    const auto b = GS2::blade<2>{::SymEngine::Expression{"b"}};

    return expect(eq(multivector{a, b}, a + b));
  };

  "formattable"_test = [] {
    return expect(eq(
        "0.3 + ae₁ + be₀₁₂",
        std::format(
            "{}",
            multivector{
                GS2::blade<>{::SymEngine::Expression{0.3}},
                GS2::blade<1>{::SymEngine::Expression{"a"}},
                GS2::blade<0, 1, 2>{::SymEngine::Expression{"b"}}})));
  };
}
