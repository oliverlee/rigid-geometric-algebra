#pragma once

#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>

namespace sym {
namespace op {

template <class D>
class op_base
{
  op_base() = default;
  friend D;

  template <class D2>
  friend constexpr auto
  operator==(const op_base&, const op_base<D2>&) noexcept -> bool
  {
    return std::is_same_v<D, D2>;
  }

public:
  [[nodiscard]]
  static constexpr auto name() noexcept -> std::string_view
  {
    using namespace std::string_view_literals;

    static constexpr auto header = "[D = "sv;
    static constexpr auto footer = "_t]"sv;

    auto sv = std::string_view{__PRETTY_FUNCTION__};
    sv.remove_suffix(footer.size());
    sv.remove_prefix(sv.rfind(header) + header.size());

    return sv;
  }
};

template <class Bop>
struct fold_adaptor : Bop
{
  using Bop::operator();

  template <class T1, class T2, class... Ts>
    requires (sizeof...(Ts) != 0)
  static constexpr auto operator()(T1&& t1, T2&& t2, Ts&&... ts) noexcept(
      noexcept(std::declval<fold_adaptor>()(
          std::declval<Bop>()(std::forward<T1>(t1), std::forward<T2>(t2)),
          std::forward<Ts>(ts)...)))
      -> decltype(std::declval<fold_adaptor>()(
          std::declval<Bop>()(std::forward<T1>(t1), std::forward<T2>(t2)),
          std::forward<Ts>(ts)...))
  {
    return fold_adaptor{}(
        Bop{}(std::forward<T1>(t1), std::forward<T2>(t2)),
        std::forward<Ts>(ts)...);
  }
};

inline constexpr struct identity_t : std::identity, op_base<identity_t>
{
  explicit identity_t() = default;
} identity{};

inline constexpr struct plus_t : fold_adaptor<std::plus<>>, op_base<plus_t>
{
  explicit plus_t() = default;
} plus{};

}  // namespace op

template <class F>
concept operation =
    std::derived_from<F, op::op_base<F>> and  //
    std::default_initializable<F> and         //
    std::is_empty_v<F> and                    //
    requires {
      { F::name() } -> std::same_as<std::string_view>;
    };

}  // namespace sym
