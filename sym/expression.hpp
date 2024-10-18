#pragma once

#include "sym/operation.hpp"
#include "sym/symbol.hpp"

#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <format>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace sym {

class [[nodiscard]] expression
{
  class any_operation
  {
    enum class member : std::uint8_t
    {
      name,
    };

    union op_args
    {
      std::string_view name;
    };

    using erased_op_type = auto (*)(member, op_args&) -> void;
    erased_op_type op_;

    // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

    template <class F>
    struct erased
    {
      static constexpr auto op(member which, op_args& args) noexcept
      {
        switch (which) {
          case member::name:
            args.name = F::name();
            break;
        }
      }
    };

  public:
    template <operation F>
    constexpr any_operation(F) : op_{erased<F>::op}
    {}

    [[nodiscard]]
    constexpr auto name() const noexcept -> std::string_view
    {
      auto args = op_args{std::string_view{}};
      op_(member::name, args);
      return args.name;
    }

    // NOLINTEND(cppcoreguidelines-pro-type-union-access)
  };

  using args_type = std::variant<std::vector<expression>, symbol>;

  any_operation op_;
  args_type args_;

  template <class Self>
  [[nodiscard]]
  constexpr auto subexpressions(this Self& self) noexcept -> auto&
  {
    auto p = std::get_if<0>(&self.args_);
    assert(p != nullptr);
    return *p;
  }

public:
  constexpr expression(symbol s) noexcept
      : op_{op::identity}, args_{std::in_place_index<1>, s}
  {}

  template <operation Op, std::same_as<symbol>... Symbols>
    requires (Op{} != op::identity) and (sizeof...(Symbols) > 0)
  constexpr expression(Op op, Symbols... symbols)
      : op_{op}, args_{std::in_place_index<0>}
  {
    subexpressions().reserve(sizeof...(Symbols));

    for (auto s : std::array{symbols...}) {
      subexpressions().emplace_back(s);
    }
  }

  [[nodiscard]]
  constexpr auto op() const noexcept -> const any_operation&
  {
    return op_;
  }

  [[nodiscard]]
  constexpr auto args() const noexcept -> const args_type&
  {
    return args_;
  }
};

}  // namespace sym

template <class Char>
struct ::std::formatter<::sym::expression, Char>
    : std::formatter<std::string_view, Char>
{
  template <class O>
  constexpr auto
  format(const ::sym::expression& expr, std::basic_format_context<O, Char>& ctx)
      const
  {
    auto out = std::format_to(ctx.out(), "{{{}: ", expr.op().name());

    if (const auto* args = std::get_if<0>(&expr.args())) {
      out = std::format_to(out, "{}", *args);
    }
    if (const auto* args = std::get_if<1>(&expr.args())) {
      out = std::format_to(out, "{}", *args);
    }

    return std::format_to(out, "}}");
  }
};
