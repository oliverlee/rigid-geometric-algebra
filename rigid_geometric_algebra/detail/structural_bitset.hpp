#pragma once

#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cstdint>

namespace rigid_geometric_algebra::detail {

/// simplification of std::bitset defined as a structural type
/// @tparam N number of bits
///
template <std::size_t N>
struct structural_bitset
{
  static_assert(N <= 8);
  using value_type = std::uint8_t;

  /// unsigned integer representation of the data
  ///
  value_type value{};

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
    assert(pos < 8UZ and "`pos` exceeds number of bits in `structural_bitset`");
    self.value |= std::uint8_t{1} << pos;
    return std::forward<Self>(self);
  }

  /// returns the number of bits the `structural_bitset` holds
  ///
  [[nodiscard]]
  constexpr auto size() const noexcept -> std::size_t
  {
    return N;
  }

  /// returns the number of bits set to `true`
  ///
  [[nodiscard]]
  constexpr auto count() const noexcept -> std::size_t
  {
    return std::size_t(std::popcount(value));
  }

  /// returns the value of a bit
  /// @param pos bit position
  /// @pre `pos` < `N` is `true`
  /// @return `true` is the bit at position `pos` is `true`; otherwise `false`.
  ///
  [[nodiscard]]
  constexpr auto test(std::size_t pos) const -> bool
  {
    assert(pos < 8UZ and "`pos` exceeds number of bits in `structural_bitset`");
    return (value & (std::uint8_t{1} << pos)) != 0;
  }

  /// returns the binary OR between two `structural_bitset`s
  ///
  friend constexpr auto
  operator|(structural_bitset lhs, structural_bitset rhs) noexcept
      -> structural_bitset
  {
    return {lhs.value |= rhs.value};
  }

  /// returns an unsigned integer representation of the data
  ///
  [[nodiscard]]
  constexpr auto to_unsigned() const noexcept -> value_type
  {
    return value;
  }

  /// equality comparison
  ///
  friend constexpr auto
  operator==(structural_bitset, structural_bitset) noexcept -> bool = default;
};

}  // namespace rigid_geometric_algebra::detail
