#pragma once

#include "sym/operation.hpp"
#include "sym/symbol.hpp"

#include <array>
#include <cassert>
#include <concepts>
#include <format>
#include <memory>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace sym {

class [[nodiscard]] expression
{
  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
  class any_operation
  {
    struct concept_t
    {
      virtual ~concept_t() = default;
      [[nodiscard]]
      constexpr virtual auto name_() const noexcept -> std::string_view = 0;
    };

    template <class F>
    struct model_t : concept_t
    {
      [[nodiscard]]
      constexpr auto name_() const noexcept -> std::string_view override
      {
        return F::name();
      }
    };

    std::unique_ptr<const concept_t> op_;

  public:
    template <class F>
    constexpr any_operation(F) : op_{std::make_unique<model_t<F>>()}
    {}

    [[nodiscard]]
    constexpr auto name() const noexcept -> std::string_view
    {
      return op_->name_();
    }
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
  constexpr expression(symbol s)
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
