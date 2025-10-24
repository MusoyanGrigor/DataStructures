#pragma once

#include <iterator>
#include <type_traits>
#include "iterator.hpp"

namespace it {
    template <typename InputIt>
    constexpr auto distance(InputIt first, InputIt last) {
        using category = std::iterator_traits<InputIt>::iterator_category;
        using diff_t = std::iterator_traits<InputIt>::difference_type;

        if constexpr (std::is_base_of_v<random_access_iterator_tag, category>) {
            return static_cast<diff_t>(last - first);
        } else {
            diff_t count = 0;
            while (first != last) {
                ++first;
                ++count;
            }
            return count;
        }
    }
}
