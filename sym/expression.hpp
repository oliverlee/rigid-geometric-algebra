#pragma once

#include "sym/operation.hpp"
#include "sym/symbol.hpp"

#include <array>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string_view>
#include <type_traits>
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

    [[nodiscard]]
    friend auto operator==(any_operation, any_operation) -> bool = default;

    [[nodiscard]]
    friend constexpr auto
    operator<=>(any_operation lhs, any_operation rhs) noexcept
        -> std::strong_ordering
    {
      if (lhs == rhs) {
        return std::strong_ordering::equal;
      }

      const auto order = lhs.name() <=> rhs.name();
      assert(order != std::strong_ordering::equal);

      return order;
    }
  };

  using args_type = std::variant<std::vector<expression>, symbol>;

  template <std::size_t I>
  struct alternative_t
  {
    template <class Self>
    static constexpr auto operator()(Self& self) -> auto&
    {
      auto p = std::get_if<I>(&self.args_);
      assert(p != nullptr);
      return *p;
    }
  };

  template <std::size_t I>
  static constexpr auto alternative = alternative_t<I>{};

  template <std::size_t I>
  struct index
      : std::in_place_index_t<I>,
        std::integral_constant<std::size_t, I>
  {
    index() = default;
  };

  static constexpr auto subexpressions_index = index<0>{};
  static constexpr auto symbol_index = index<1>{};

  any_operation op_;
  args_type args_;

public:
  constexpr expression(symbol s) noexcept
      : op_{op::identity}, args_{symbol_index, s}
  {}

  template <operation Op, std::same_as<symbol>... Symbols>
    requires (Op{} != op::identity) and (sizeof...(Symbols) > 0)
  constexpr expression(Op op, Symbols... symbols)
      : op_{op}, args_{subexpressions_index}
  {
    auto& subexpr = alternative<subexpressions_index>(*this);
    subexpr.reserve(sizeof...(Symbols));

    for (auto s : std::array{symbols...}) {
      subexpr.emplace_back(s);
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

  [[nodiscard]]
  friend auto
  operator==(const expression&, const expression&) -> bool = default;

  [[nodiscard]]
  friend constexpr auto
  operator<=>(const expression& lhs, const expression& rhs) noexcept
      -> std::strong_ordering
  {
    if (lhs == rhs) {
      return std::strong_ordering::equal;
    }

    const auto relation = [&lhs, &rhs](auto proj) {
      const auto order = std::invoke(proj, lhs) <=> std::invoke(proj, rhs);
      assert(order != std::strong_ordering::equal);
      return order;
    };

    if (lhs.op() == rhs.op()) {
      if (lhs.args().index() == subexpressions_index) {
        return relation(alternative<subexpressions_index>);
      }
      return relation(alternative<symbol_index>);
    }

    return relation(&expression::op);
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
