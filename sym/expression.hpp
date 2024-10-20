#pragma once

#include "sym/constant.hpp"
#include "sym/operation.hpp"
#include "sym/sym_invocable.hpp"
#include "sym/symbol.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string_view>
#include <tuple>
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

  template <std::size_t I>
  struct index
      : std::in_place_index_t<I>,
        std::integral_constant<std::size_t, I>
  {
    index() = default;
  };

  static constexpr auto subexpressions_index = index<0>{};
  static constexpr auto constant_index = index<1>{};
  static constexpr auto symbol_index = index<2>{};

public:
  using args_type = std::variant<std::vector<expression>, constant, symbol>;

private:
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

  any_operation op_;
  args_type args_;

public:
  constexpr expression() noexcept : expression{constant{}} {}

  constexpr explicit expression(symbol s) noexcept
      : op_{op::identity}, args_{symbol_index, s}
  {}

  template <class Constant>
    requires std::constructible_from<constant, Constant>
  constexpr explicit expression(Constant c) noexcept
      : op_{op::identity}, args_{constant_index, c}
  {}

  template <operation Op, class... Args>
    requires sym_invocable<Op, Args...> and
                 (not std::same_as<Op, op::identity_t>)
  constexpr expression(Op op, Args&&... args)
      : op_{op}, args_{subexpressions_index}
  {
    auto& subexpr = alternative<subexpressions_index>(*this);
    subexpr.reserve(sizeof...(Args));

    std::ignore =
        ((subexpr.emplace_back(std::forward<Args>(args)), true) and ...);
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

    const auto order =
        (lhs.op() != rhs.op())
            ? lhs.op() <=> rhs.op()
            : lhs.args() <=> rhs.args();

    assert(order != 0);
    return order;
  }
};

}  // namespace sym

template <class Char>
struct ::std::formatter<::sym::expression, Char>
    : std::formatter<std::string_view, Char>
{
private:
  template <std::size_t I>
  [[nodiscard]]
  static constexpr auto
  format_if(std::integral_constant<std::size_t, I>, auto& out, const auto& args)
      -> bool
  {
    const auto p = std::get_if<I>(&args);

    if (p == nullptr) {
      return false;
    }

    out = std::format_to(out, "{}", *p);
    return true;
  }

public:
  template <class O>
  constexpr auto
  format(const ::sym::expression& expr, std::basic_format_context<O, Char>& ctx)
      const
  {
    static constexpr auto N = std::variant_size_v<::sym::expression::args_type>;

    auto out = std::format_to(ctx.out(), "{{{}: ", expr.op().name());

    [&]<std::size_t... I>(std::index_sequence<I...>) {
      std::ignore =
          (format_if(
               std::integral_constant<std::size_t, I>{}, out, expr.args()) or
           ...);
    }(std::make_index_sequence<N>{});

    return std::format_to(out, "}}");
  }
};
