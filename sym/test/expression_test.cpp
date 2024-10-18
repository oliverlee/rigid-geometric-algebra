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

  "copyable"_ctest = [] {
    using sym::symbol;
    using sym::expression;

    constexpr auto x1 = expression{"x"_s};
    constexpr auto x2 = x1;

    return expect(
        eq(x1.op().name(), "sym::op::identity") and
        eq(x1.op().name(), x2.op().name()) and
        std::get<symbol>(x1.args()) == std::get<symbol>(x2.args()));
  };

  "formattable"_test = [] {
    using sym::expression;
    using sym::op::plus;

    constexpr auto x_expr = "{sym::op::identity: x}";

    return expect(
        eq(x_expr, std::format("{}", expression{"x"_s})) and
        eq(std::format("{{sym::op::plus: [{0}, {0}]}}", x_expr),
           std::format("{}", expression{plus, "x"_s, "x"_s})));
  };
}
