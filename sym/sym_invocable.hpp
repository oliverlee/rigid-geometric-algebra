#pragma once

#include "sym/eval_type.hpp"
#include "sym/forward.hpp"

#include <concepts>

namespace sym {

template <class Op, class... Args>
concept sym_invocable =
    (std::constructible_from<expression, Args> and ...) and
    std::regular_invocable<const Op&, eval_type_t<Args>...>;

}  // namespace sym
