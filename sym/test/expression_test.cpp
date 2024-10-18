#include "skytest/skytest.hpp"
#include "sym/sym.hpp"

#include <format>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;

  using namespace sym::literals;

  "expression formattable"_test = [] {
    using sym::expression;
    using sym::op::plus;

    constexpr auto x_expr = "{sym::op::identity: x}";

    return expect(
        eq(x_expr, std::format("{}", expression{"x"_s})) and
        eq(std::format("{{sym::op::plus: [{0}, {0}]}}", x_expr),
           std::format("{}", expression{plus, "x"_s, "x"_s})));
  };
}
