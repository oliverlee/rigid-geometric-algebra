#include <format>
#include <iostream>
#include <symengine/expression.h>
#include <symengine/format.hpp>

auto main() -> int
{
  using SymEngine::Expression;

  auto x = Expression{"x"};
  auto ex = pow(x + sqrt(Expression{2}), 6);

  std::cout << std::format("{}\n", ex);
}
