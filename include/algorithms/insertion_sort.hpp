#pragma once

#include <functional>

namespace st {
    template <typename Container, typename Compare = std::less<>>
    void insertion_sort(Container &c, Compare comp = Compare{}) {
        for (size_t i = 1; i < c.size(); ++i) {
            auto key = c[i];
            size_t j = i;
            while (j > 0 && comp(key, c[j - 1])) {
                c[j] = c[j - 1];
                --j;
            }
            c[j] = key;
        }
    }
}