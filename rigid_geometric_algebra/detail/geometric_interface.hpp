#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

#include <array>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra::detail {

/// Base type helper for geometric types
/// @tparam V multivector type
///
/// Define a multivector data member and provides definitions for members common
/// to geometric types.
///
template <detail::multivector V>
class geometric_interface
{
  V values_;

public:
  /// geometric interface type
  ///
  using geometric_interface_type = geometric_interface;

  /// algebra type
  ///
  using algebra_type = algebra_type_t<V>;

  /// blade scalar type
  ///
  using value_type = algebra_field_t<algebra_type>;

  /// multivector type
  ///
  using multivector_type = V;

  template <bool Const>
  class iterator
  {
    using parent_type = std::
        conditional_t<Const, const geometric_interface, geometric_interface>;
    using index_type = std::uint8_t;

    parent_type* parent_{};
    index_type index_{};

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<Const, const value_type, value_type>;
    using reference = std::add_lvalue_reference_t<value_type>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::add_pointer_t<value_type>;

    iterator() = default;

    constexpr iterator(parent_type& p, index_type i) : parent_{&p}, index_{i}
    {
      detail::precondition(i <= multivector_type::size);
    }

    constexpr auto operator*() const -> reference
    {
      detail::precondition(parent_ != nullptr);
      detail::precondition(index_ < multivector_type::size);

      const auto refs =
          [&v = this->parent_->multivector()]<std::size_t... Is>(
              std::index_sequence<Is...>) {
            return std::array{std::ref(v.template get<Is>().coefficient)...};
          }(std::make_index_sequence<multivector_type::size>{});

      return refs[index_];
    };

    constexpr auto operator++() -> iterator&
    {
      detail::precondition(index_ < multivector_type::size);

      ++index_;
      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    constexpr auto operator--() -> iterator&
    {
      detail::precondition(index_ > 0);

      --index_;
      return *this;
    }

    constexpr auto operator--(int) -> iterator
    {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    friend constexpr auto
    operator-(iterator i, iterator j) noexcept -> difference_type
    {
      return static_cast<difference_type>(i.index_) -
             static_cast<difference_type>(j.index_);
    }

    constexpr auto operator+=(difference_type n) -> iterator&
    {
      const auto i = static_cast<difference_type>(index_) + n;

      detail::precondition(i >= 0);
      detail::precondition(
          i <= static_cast<difference_type>(multivector_type::size));

      index_ = static_cast<index_type>(i);
      return *this;
    }

    friend constexpr auto operator+(iterator j, difference_type n) -> iterator
    {
      j += n;
      return j;
    }

    friend constexpr auto operator+(difference_type n, iterator j) -> iterator
    {
      return j + n;
    }

    constexpr auto operator-=(difference_type n) -> iterator&
    {
      return *this += -n;
    }

    friend constexpr auto operator-(iterator j, difference_type n) -> iterator
    {
      j -= n;
      return j;
    }

    constexpr auto operator[](difference_type n) const -> reference
    {
      return *(*this + n);
    }

    constexpr friend auto
    operator<=>(iterator i, iterator j) -> std::strong_ordering
    {
      detail::precondition(i.parent_ == j.parent_);
      return i.index_ <=> j.index_;
    }

    friend auto operator==(iterator i, iterator j) -> bool = default;
  };

  /// const iterator alias
  ///
  using const_iterator = iterator<true>;

  /// construct a zero geometric type
  ///
  geometric_interface() = default;

  /// construct from multivector
  ///
  template <detail::decays_to<multivector_type> T>
  constexpr explicit geometric_interface(T&& v) : values_{std::forward<T>(v)}
  {}

  /// construct a geometric type, specifying the coefficients
  /// @tparam Ts parameter pack of types
  /// @param coeffs coefficient values
  ///
  template <class... Ts>
    requires (sizeof...(Ts) == multivector_type::size()) and
             (std::constructible_from<value_type, Ts> and ...)
  constexpr geometric_interface(Ts&&... coeffs)
      : values_{std::forward<Ts>(coeffs)...}
  {}

  /// initializer list constructor
  /// @param il initializer list of values
  ///
  /// @note allows conversions to `value_type` is floating point and if the
  ///   source can be stored exactly
  /// @see
  /// https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  /// @pre `il.size() == multivector::size` is `true`
  ///
  constexpr geometric_interface(std::initializer_list<value_type> il)
    requires std::floating_point<value_type>
      : values_{[il] {
          detail::precondition(
              il.size() == multivector_type::size,
              detail::contract_violation_handler{
                  "size of initializer_list ({}) must match underlying "
                  "multivector size ({})",
                  il.size(),
                  multivector_type::size()});
          return il;
        }()}
  {}

  /// access the underlying `multivector`
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto multivector(this Self&& self) noexcept
      -> decltype(std::forward_like<Self&&>(self.values_))
  {
    return std::forward_like<Self&&>(self.values_);
  }

  /// coefficient access
  ///
  /// @{

  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto begin(this Self&& self) noexcept
      -> iterator<std::is_const_v<std::remove_reference_t<Self>>>
  {
    return {self, 0};
  }

  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto end(this Self&& self) noexcept
      -> iterator<std::is_const_v<std::remove_reference_t<Self>>>
  {
    return {self, multivector_type::size};
  }

  /// @}

  /// coefficient access
  /// @tparam Self `this` type
  /// @param self explicit `this` parameter
  /// @param i coefficient to access
  ///
  /// returns `get<blade<algebra_type, i>>(point.multivector())`
  /// (reference to `scalar_type`)
  ///
  template <class Self>
  constexpr auto
  operator[](this Self&& self, std::size_t i) -> std::conditional_t<
      std::is_lvalue_reference_v<Self&&>,
      std::ranges::range_reference_t<Self&&>,
      std::ranges::range_rvalue_reference_t<Self&&>>
  {
    detail::precondition(i < multivector_type::size);

    using D = iterator<true>::difference_type;
    using R = std::conditional_t<
        std::is_lvalue_reference_v<Self&&>,
        std::ranges::range_reference_t<Self&&>,
        std::ranges::range_rvalue_reference_t<Self&&>>;

    auto&& ref = std::forward<Self>(self).begin()[static_cast<D>(i)];
    return static_cast<R>(ref);
  }

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const geometric_interface&, const geometric_interface&)
      -> bool = default;

  /// @}
};

template <class T>
concept geometric =
    requires { typename std::remove_cvref_t<T>::geometric_interface_type; } and
    detail::is_specialization_of_v<
        typename std::remove_cvref_t<T>::geometric_interface_type,
        detail::geometric_interface>;

}  // namespace rigid_geometric_algebra::detail

template <class V, class Char>
struct ::std::
    formatter<::rigid_geometric_algebra::detail::geometric_interface<V>, Char>
    : ::std::formatter<
          typename ::rigid_geometric_algebra::detail::geometric_interface<
              V>::multivector_type,
          Char>
{
  // https://github.com/llvm/llvm-project/issues/66466
  template <class O>
  constexpr auto
  format(const ::rigid_geometric_algebra::detail::geometric_interface<V>& p,
         ::std::basic_format_context<O, Char>& ctx) const
  {
    return ::std::formatter<
        typename ::rigid_geometric_algebra::detail::geometric_interface<
            V>::multivector_type,
        Char>::format(p.multivector(), ctx);
  }
};

template <class V>
struct ::glz::meta<::rigid_geometric_algebra::detail::geometric_interface<V>>
{
  template <class P>
  struct geometric_wrapper
  {
    std::reference_wrapper<P> point;
  };

  template <class T>
  geometric_wrapper(T&) -> geometric_wrapper<T>;

  static constexpr auto value = [](auto& self) {
    return geometric_wrapper{self.multivector().as_tuple()};
  };
};
