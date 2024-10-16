#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <optional>
#include <ranges>

namespace rigid_geometric_algebra::detail {

/// sorts a range and returns the number of calls to `std::ranges::iter_swap`
///
inline constexpr class
{
  /// iter_swap function object that counts the number of invocations
  ///
  struct counted_iter_swap
  {
    std::size_t count{};

    constexpr auto operator()(auto it1, auto it2) & -> void
    {
      std::ranges::iter_swap(it1, it2);
      ++count;
    }
  };

  /// push an element onto a heap
  /// @param first, last range representing a heap
  ///
  /// Pushes element at `last - 1` onto a max heap defined by the range
  /// `[first, last - 1)`.
  ///
  /// @return number of calls to `iter_swap` when pushing on element at
  // `last - 1`
  ///
  /// @see https://en.wikipedia.org/wiki/Binary_heap
  /// @see https://en.cppreference.com/w/cpp/algorithm/ranges/push_heap
  ///
  template <std::random_access_iterator I>
  static constexpr auto counted_push_heap(I first, I last) -> std::size_t
  {
    if (std::ranges::distance(first, last) <= 1) {
      return 0UZ;
    }

    auto iter_swap = counted_iter_swap{};

    /// obtains the iterator corresponding to the parent
    /// @param it iterator to an element in a heap
    /// @pre it is a valid iterator in the range [first + 1, last - 1]
    ///
    const auto parent = [first](I it) -> I {
      const auto n = (std::distance(first, it) - 1) / 2;
      return first + n;
    };

    // adjust `last` to *not* point past the last element and then obtain the
    // parent iterator
    auto p = parent(--last);

    while (*p < *last) {
      iter_swap(p, last);
      last = p;
      if (p == first) {
        break;
      }
      p = parent(last);
    }

    return iter_swap.count;
  }

  /// pop an element from a heap
  /// @param first, last range representing a heap
  ///
  /// Swaps elements at `first` and `last - 1` and updates range
  /// `[first, last - 1)` to be a max heap.
  ///
  /// @return number of calls to `iter_swap`
  ///
  /// @see https://en.wikipedia.org/wiki/Binary_heap
  /// @see https://en.cppreference.com/w/cpp/algorithm/ranges/pop_heap
  ///
  template <std::random_access_iterator I>
  static constexpr auto counted_pop_heap(I first, I last) -> std::size_t
  {
    if (std::ranges::distance(first, last) <= 1) {
      return 0UZ;
    }

    auto iter_swap = counted_iter_swap{};
    iter_swap(first, --last);

    /// obtains the iterator corresponding to the largest child
    /// @param it iterator to an element in a heap
    /// @pre it is a valid iterator in the range `[first, last - 1)`
    ///
    const auto largest_child =
        [first, length = std::ranges::distance(first, last)](I it)
        -> std::optional<I> {
      const auto n = 2 * std::ranges::distance(first, it);
      auto largest = it;

      if (((n + 1) < length) and (*largest < first[n + 1])) {
        largest = first + (n + 1);
      }
      if (((n + 2) < length) and (*largest < first[n + 2])) {
        largest = first + (n + 2);
      }

      if (largest == it) {
        return {};
      }

      return {largest};
    };

    while (auto c = largest_child(first)) {
      iter_swap(*c, first);
      first = *c;
    }

    return iter_swap.count;
  }

  static constexpr auto
  counted_make_heap(std::ranges::random_access_range auto&& rng) -> std::size_t
  {
    auto count = 0UZ;

    for (auto it = rng.begin();; ++it) {
      count += counted_push_heap(rng.begin(), it);
      if (it == rng.end()) {
        break;
      }
    }

    return count;
  }

  static constexpr auto
  counted_sort_heap(std::ranges::random_access_range auto&& rng) -> std::size_t
  {
    auto count = 0UZ;

    for (auto it = rng.end(); it != rng.begin(); --it) {
      count += counted_pop_heap(rng.begin(), it);
    }

    return count;
  }

public:
  template <std::ranges::random_access_range R>
    requires std::sortable<std::ranges::iterator_t<R>>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  static constexpr auto operator()(R&& rng) -> std::size_t
  {
    auto count = counted_make_heap(rng);
    count += counted_sort_heap(rng);

    detail::postcondition(std::ranges::is_sorted(rng));
    return count;
  }
} counted_sort{};

}  // namespace rigid_geometric_algebra::detail
