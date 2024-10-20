#pragma once

#include "sym/expression.hpp"
#include "sym/operation.hpp"
#include "sym/sym_invocable.hpp"

#include <utility>

namespace sym {

template <class A1, class A2>
  requires sym_invocable<op::plus_t, A1, A2>
[[nodiscard]]
constexpr auto operator+(A1&& lhs, A2&& rhs) -> expression
{
  return {op::plus, std::forward<A1>(lhs), std::forward<A2>(rhs)};
}

}  // namespace sym
