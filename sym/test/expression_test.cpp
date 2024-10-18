#include "skytest/skytest.hpp"
#include "sym/sym.hpp"

#include <format>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::lt;
  using ::skytest::ne;

  using namespace sym::literals;
  using sym::op::plus;

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

  {
    using sym::expression;

    static constexpr auto x = expression{"x"_s};
    static constexpr auto y = expression{"y"_s};

    "erased op has total order"_ctest = [] {
      const auto xx = expression{plus, "x"_s, "x"_s};

      return expect(
          eq(x.op(), x.op()) and    //
          eq(xx.op(), xx.op()) and  //
          lt(x.op(), xx.op()));
    };

    "expressions are equality comparable"_test = [] {
      const auto xx = expression{plus, "x"_s, "x"_s};

      return expect(
          eq(x, x) and    //
          eq(y, y) and    //
          eq(xx, xx) and  //
          ne(x, y) and    //
          ne(x, xx) and   //
          ne(y, xx));
    };

    "promoted symbol expressions have total order"_ctest = [] {
      return expect(lt(x, expression{"y"_s}));
    };

    "symbol expression ordered before other operations"_ctest = [] {
      return expect(lt(x, expression{plus, "x"_s, "x"_s}));
    };

    "order of plus expressions"_ctest = [] {
      return expect(
          lt(expression{plus, "x"_s, "x"_s}, expression{plus, "x"_s, "y"_s}) and
          lt(expression{plus, "x"_s, "x"_s},
             expression{plus, "x"_s, "x"_s, "y"_s}));
    };
  }

  "formattable"_test = [] {
    using sym::expression;

    constexpr auto x_expr = "{sym::op::identity: x}";

    return expect(
        eq(x_expr, std::format("{}", expression{"x"_s})) and
        eq(std::format("{{sym::op::plus: [{0}, {0}]}}", x_expr),
           std::format("{}", expression{plus, "x"_s, "x"_s})));
  };
}
