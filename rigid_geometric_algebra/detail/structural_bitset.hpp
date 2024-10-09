#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// simplification of std::bitset defined as a structural type
/// @tparam N number of bits
///
template <std::size_t N>
struct structural_bitset
{
  static_assert(N <= 8, "this type only supports algebras up to 8 dimensions");
  using value_type = std::uint8_t;

  class const_iterator
  {
    const structural_bitset* parent_{};
    structural_bitset::value_type pos_{};

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = bool;
    using reference = bool;
    using difference_type = std::ptrdiff_t;
    using pointer = void;

    // declare but don't define
    // https://stackoverflow.com/questions/28832492/why-do-iterators-need-to-be-default-constructible
    constexpr const_iterator();

    constexpr const_iterator(
        const structural_bitset& parent, structural_bitset::value_type pos)
        : parent_{&parent}, pos_{pos}
    {
      detail::precondition(pos_ <= parent_->size());
    }

    constexpr auto operator*() const noexcept -> reference
    {
      return parent_->test(pos_);
    }
    constexpr auto operator++() & -> const_iterator&
    {
      ++pos_;
      return *this;
    }
    constexpr auto operator++(int) & -> const_iterator
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    friend auto operator==(const_iterator, const_iterator) -> bool = default;
  };

  /// the number of bits the `structural_bitset` holds
  ///
  static constexpr auto size = std::integral_constant<std::size_t, N>{};

  /// unsigned integer representation of the data
  ///
  /// @note bits at index size or higher are ignored
  ///
  value_type value_{};

  /// construct with no bits
  ///
  structural_bitset() = default;

  /// construct from bits
  /// @pre bits does not set bits at size or higher
  ///
  constexpr structural_bitset(value_type bits) : value_{bits}
  {
    static constexpr auto mask = (value_type{} - value_type{1}) << size;
    detail::precondition(
        (bits &= mask) == value_type{},
        "`bits` has bits higher than structural_bitset size");
  }

  /// sets bits to `true`
  /// @tparam Self reference to `structural_bitset` type
  /// @tparam self explicit object
  /// @tparam pos bit position
  ///
  /// Sets the bit as position `pos` to `true`.
  ///
  /// @return `self`
  ///
  /// @pre `pos` < `N` is `true`
  /// @note Requires:
  ///   `std::is_const_v<std::remove_reference_t<Self>>` is `false`
  ///
  template <class Self>
    requires (not std::is_const_v<std::remove_reference_t<Self>>)
  constexpr auto set(this Self&& self, std::size_t pos) -> Self&&
  {
    detail::precondition(
        pos < size, "`pos` exceeds number of bits in `structural_bitset`");
    self.value_ |= std::uint8_t{1} << pos;
    return std::forward<Self>(self);
  }

  /// returns the number of bits set to `true`
  ///
  [[nodiscard]]
  constexpr auto count() const noexcept -> std::size_t
  {
    return std::size_t(std::popcount(value_));
  }

  /// returns the value of a bit
  /// @param pos bit position
  /// @pre `pos` < `N` is `true`
  /// @return `true` is the bit at position `pos` is `true`; otherwise `false`.
  ///
  [[nodiscard]]
  constexpr auto test(std::size_t pos) const -> bool
  {
    detail::precondition(
        pos < size, "`pos` exceeds number of bits in `structural_bitset`");
    return (value_ & (std::uint8_t{1} << pos)) != 0;
  }

  /// returns the binary OR between two `structural_bitset`s
  ///
  friend constexpr auto
  operator|(structural_bitset lhs, structural_bitset rhs) noexcept
      -> structural_bitset
  {
    return {lhs.value_ |= rhs.value_};
  }

  /// returns the binary AND between two `structural_bitset`s
  ///
  friend constexpr auto
  operator&(structural_bitset lhs, structural_bitset rhs) noexcept
      -> structural_bitset
  {
    return {lhs.value_ &= rhs.value_};
  }

  /// returns an unsigned integer representation of the data
  ///
  [[nodiscard]]
  constexpr auto to_unsigned() const noexcept -> value_type
  {
    return value_;
  }

  /// iterators for range-based access
  ///
  /// @{

  [[nodiscard]]
  constexpr auto begin() const noexcept -> const_iterator
  {
    return {*this, 0};
  }
  [[nodiscard]]
  constexpr auto end() const noexcept -> const_iterator
  {
    return {*this, size};
  }

  /// @}

  /// equality comparison
  ///
  friend auto
  operator==(structural_bitset, structural_bitset) -> bool = default;
};

}  // namespace rigid_geometric_algebra::detail
