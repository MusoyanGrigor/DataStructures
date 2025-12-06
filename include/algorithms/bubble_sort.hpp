#pragma once

#include <functional>

namespace st {
    template <typename Container,  typename Compare = std::less<>>
    void bubble_sort(Container &c, Compare comp = Compare{}) {
        for (size_t i = 0; i < c.size(); ++i) {
            bool swapped = false;
            for (size_t j = 0; j < c.size() - i - 1; ++j) {
                if (comp(c[j + 1], c[j])) {
                    std::swap(c[j], c[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
    }
}